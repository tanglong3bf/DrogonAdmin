# DrogonAdmin

基于[Drogon框架](https://github.com/drogonframework/drogon)和vue3技术栈搭建的一个通用后台管理项目。

## 部署

### 后端

数据库：

```bash
docker run --name drogon-admin-pgsql \
      -e POSTGRES_USER=admin \
      -e POSTGRES_PASSWORD=123456 \
      -e POSTGRES_DB=drogon_admin_db \
      -p 5432:5432 \
      -v ~/docker/drogonAdminPgsqlData:/var/lib/postgresql \
      -d postgres:18.1
```

其余内容待补充

### 前端

用 docker-compose.yml 准备 nginx：

```yaml
version: '3.8'

services:
  nginx:
    image: nginx:1.31.2 # 版本任意
    container_name: da-nginx # 自定义容器名称
    ports:
      - '80:80'
      # - "443:443"
    volumes:
      - ./html:/usr/share/nginx/html # 静态资源目录
      - ./nginx:/etc/nginx # nginx 配置目录
      - ./logs:/var/log/nginx # nginx 日志目录
    restart: always # 容器退出后自动重启
    environment:
      - TZ=Asia/Shanghai
```

运行起来 nginx 前需要补充 nginx.conf：

```nginx
events {
    worker_connections 1024;
}

http {
    types {
        text/html                             html;
        text/css                              css;
        application/javascript                js;
        image/png                             png;
        image/jpeg                            jpe?g;
        image/svg+xml                         svg;
    }
    default_type  application/octet-stream;

    server {
        listen       80;
        server_name  _;

        root   /usr/share/nginx/html;
        index  index.html;

        # 静态资源缺失，不跳首页，正常返回404
        location ~* \.(svg|js|css|png|jpe?g)$ {
            try_files $uri =404;
            expires 30d;
        }

        # 匹配不到的页面自动跳首页，由路由代码接管
        location / {
            try_files $uri $uri/ /index.html;
        }
    }
}
```

编译代码并拷贝到 nginx 中：

```bash
cd ./frontend
yarn build
cp ./dist/* .../html # 此路径由docker配置
```

最后运行起来:

```bash
docker-compose up
```

## 前端中使用的默认头像来源

[https://www.iconfont.cn/illustrations/detail?cid=33878](https://www.iconfont.cn/illustrations/detail?cid=33878)

## 开源协议 (License)

本项目基于 **MIT** 许可证开源，详情请参阅 [LICENSE](LICENSE) 文件。

### 第三方组件声明 (Third-Party Notices)

本项目中包含以下第三方开源组件：

- **BCryptCpp** - 位于 `/backend/common/util/third_party/BCryptCpp/`，基于 **ISC** 许可证开源。
