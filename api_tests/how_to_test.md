# 接口测试

## 安装相关的依赖包

```bash
sudo apt install python3-pytest python3-jsonpath-ng python3-requests \
python3-jsonpath-ng python3-psycopg2 -y
```

> 当然用pip安装也不是不行，但是我不会。

## 把后端运行起来

### 准备好基础数据

准备好一个pgsql的数据库，建议参照README.md中的docker命令。

```bash
docker run --name drogon-admin-pgsql \
      -e POSTGRES_USER=admin \
      -e POSTGRES_PASSWORD=123456 \
      -e POSTGRES_DB=drogon_admin_db \
      -p 5432:5432 \
      -v ~/docker/drogonAdminPgsqlData:/var/lib/postgresql \
      -d postgres:18.1
```

执行 `../backend/init.sql` 中的SQL语句。

### 准备数据模型类

1. 在`../backend/domain/models/model.json`中配置好连接信息，需要与上一步准备的数据库连接信息一致
2. 运行以下命令生成数据模型类：

```bash
cd ../backend/domain/models
drogon_ctl create model .
```

### 准备好空的数据库

建议使用一个空的数据库用于测试。

名字：`da_test_db`

### 运行后端

1. 修改 `../backend/config.json` 中的数据库连接信息，连接到`da_test_db`。
2. 运行后端：

```bash
cd ../backend
mkdir build
cd build
cmake ..
make -j(nproc)
./DrogonAdminBackend
```

## 修改`api_test_runner.py`文件中的数据库连接信息

```python
# 我用的pgsql数据库连接信息如下，请根据你的实际情况修改
# 用于准备每一组测试的数据库环境，所以建议单独准备一个空的数据库用于测试
PG_CONFIG = {
    "host": "localhost",
    "user": "admin",
    "password": "123456",
    "dbname": "da_test_db",
    "port": 5432
}
```

## 运行测试

```bash
# 运行测试
pytest api_test_runner.py
```

## 过程

需求
测试点
测试用例
