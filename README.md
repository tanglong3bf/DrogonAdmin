# DrogonAdmin

基于[Drogon框架](https://github.com/drogonframework/drogon)和vue3技术栈搭建的一个通用后台管理项目。

我使用的数据库：

```bash
docker run --name drogon-admin-pgsql \
      -e POSTGRES_USER=admin \
      -e POSTGRES_PASSWORD=123456 \
      -e POSTGRES_DB=drogon_admin_db \
      -p 5432:5432 \
      -v ~/docker/drogonAdminPgsqlData:/var/lib/postgresql \
      -d postgres:18.1
```

## 开源协议 (License)

本项目基于 **MIT** 许可证开源，详情请参阅 [LICENSE](LICENSE) 文件。

### 第三方组件声明 (Third-Party Notices)

本项目中包含以下第三方开源组件：

- **BCryptCpp** - 位于 `/backend/common/util/third_party/BCryptCpp/`，基于 **ISC** 许可证开源。
