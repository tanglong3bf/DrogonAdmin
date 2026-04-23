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

## 部门管理

## 角色管理
