import pytest
import requests
import yaml
from jsonpath_ng import parse
from pathlib import Path
import psycopg2
from psycopg2 import OperationalError
from jsonschema import validate, ValidationError, RefResolver
import re
from typing import Any, Dict

"""
PG 配置
"""
PG_CONFIG = {
    "host": "localhost",
    "user": "admin",
    "password": "123456",
    "dbname": "da_test_db",
    "port": 5432
}


def load_schema(yaml_path):
    """
    # 读取 YAML 结构定义
    """
    with open(yaml_path, "r", encoding="utf-8") as f:
        return yaml.safe_load(f)


def validate_schema(data, schema, full_schema=None):
    """
    使用标准 jsonschema 库进行校验，支持外部 definitions
    """
    try:
        if full_schema:
            # 创建一个解析器，使用完整 schema 作为引用上下文
            resolver = RefResolver.from_schema(full_schema)
            validate(instance=data, schema=schema, resolver=resolver)
        else:
            validate(instance=data, schema=schema)
    except ValidationError as e:
        error_path = " -> ".join(map(str, e.path))
        assert False, f"Schema 校验失败：路径 [{error_path}]，错误：{e.message}"


def read_yaml(yaml_path):
    """
    读取 YAML 测试用例文件
    :param yaml_path: YAML 文件相对路径
    :return: 测试用例列表（可直接用于 pytest 参数化）
    """
    try:
        with open(yaml_path, 'r', encoding='utf-8') as f:
            # 加载 YAML
            data = yaml.safe_load(f)

        if not isinstance(data, dict):
            raise ValueError("YAML 文件内容必须是一个字典")
        return data
    except Exception as e:
        raise RuntimeError(f"YAML 读取失败：{e}")


def run_sql_file(sql_file: str):
    """
    执行SQL初始化数据，增加异常处理+连接关闭
    """
    sql_path = Path(__file__).parent / "sql" / sql_file
    conn = None
    cursor = None
    try:
        sql = sql_path.read_text(encoding="utf-8")
        conn = psycopg2.connect(**PG_CONFIG)
        cursor = conn.cursor()
        cursor.execute(sql)
        conn.commit()
    except OperationalError:
        raise RuntimeError("数据库连接失败，请检查配置")
    except Exception as e:
        raise RuntimeError(f"SQL执行失败：{e}")
    finally:
        # 确保连接关闭，防止泄露
        if cursor:
            cursor.close()
        if conn:
            conn.close()


# 变量占位符正则：匹配 ${变量名} 格式
_VAR_PATTERN = re.compile(r"\$\{(\w+)\}")

# 全局变量池（存储 extract 提取的变量，后续替换使用）
var_pool: Dict[str, Any] = {}


def replace_vars(data: Any, pool: Dict[str, Any]) -> Any:
    """
    递归替换数据中的 ${变量名} 占位符
    支持 dict、list、str 三种可嵌套类型，其他类型直接返回
    """
    if isinstance(data, dict):
        return {key: replace_vars(value, pool) for key, value in data.items()}
    elif isinstance(data, list):
        return [replace_vars(item, pool) for item in data]
    elif isinstance(data, str):
        def _substitute(match: re.Match[str]) -> str:
            var_name = match.group(1)
            if var_name not in pool:
                raise RuntimeError(
                    f"变量 [{var_name}] 未定义，请确保前置用例已通过 extract 提取该变量"
                )
            return str(pool[var_name])
        return _VAR_PATTERN.sub(_substitute, data)
    else:
        # 数字、布尔、None 等非字符串类型直接返回
        return data


auth_datas: dict = read_yaml("cases/auth_test_case.yaml")
# 提取用例 name 做展示名
case_names = [case_item["name"] for case_item in auth_datas["auth_cases"]]

dept_datas: dict = read_yaml("cases/dept_test_case.yaml")
case_names += [case_item["name"] for case_item in dept_datas["dept_cases"]]

role_datas: dict = read_yaml("cases/role_test_case.yaml")
case_names += [case_item["name"] for case_item in role_datas["role_cases"]]

user_datas: dict = read_yaml("cases/user_test_case.yaml")
case_names += [case_item["name"] for case_item in user_datas["user_cases"]]


@pytest.mark.parametrize("case_item", auth_datas["auth_cases"] + dept_datas["dept_cases"] + role_datas["role_cases"] + user_datas["user_cases"],  ids=case_names)
def test_dept_cases(case_item):
    # 部分接口需要提前准备数据
    if "sql_file" in case_item:
        run_sql_file(case_item["sql_file"])

    # 变量替换
    url = replace_vars(case_item["url"], var_pool)
    method = case_item["method"]

    need_token = case_item.get("need_token", True)
    # 根据字段判断是否需要token
    headers = {}
    # 如果没有标记「不需要auth」，则添加token（默认需要token）
    if need_token:
        headers['Authorization'] = 'Bearer ' + var_pool['token']

    params = replace_vars(case_item.get("params", {}), var_pool)
    req_json = replace_vars(case_item.get("json", {}), var_pool)

    # 发起请求
    resp = requests.request(
        url=url,
        method=method,
        headers=headers,
        params=params,
        json=req_json
    )

    # 检查响应状态码
    assert resp.status_code == case_item["expect"]["status_code"]

    # json响应体
    res_json = None
    # 仅在需要检查json时才获取响应体
    if "validate_schema" in case_item and case_item["validate_schema"] or "assert_jsonpath" in case_item["expect"]:
        try:
            res_json = resp.json()
        except Exception:
            assert False, "响应体不是合法JSON格式"

    # 根据schema检查响应体
    if "validate_schema" in case_item and case_item["validate_schema"]:
        schema_file = case_item.get("validate_schema")
        if schema_file:
            full_schema = load_schema("schemas/" + schema_file)
            if isinstance(full_schema, dict):
                validate_schema(res_json, full_schema, full_schema)

    # 根据jsonpath检查响应体
    if "assert_jsonpath" in case_item["expect"]:
        for key, expect_value in case_item["expect"]["assert_jsonpath"].items():
            # 单独处理长度检查
            if key.startswith("len:"):
                json_path = key.replace("len:", "")
                expr = parse(json_path)
                actual_value = [match.value for match in expr.find(res_json)]
                real_array = actual_value[0] if actual_value else []
                assert len(real_array) == expect_value, f"长度断言失败！路径：{
                    json_path}，预期长度：{expect_value}，实际长度：{len(real_array)}"
            # 标准的jsonpath检查
            else:
                expr = parse(key)
                actual_value = [match.value for match in expr.find(res_json)]
                actual_value = actual_value[0] if actual_value else actual_value

                support_ops = ("lt ", "gt ", "le ", "ge ", "eq ")
                # 单独处理支持比较运算的断言，格式示例： "gt 10" 表示实际值应该大于10
                if isinstance(expect_value, str) and expect_value.startswith(support_ops):
                    op, val_str = expect_value.split(maxsplit=1)
                    try:
                        val = float(val_str)
                    except ValueError:
                        raise AssertionError(
                            f"字段 {key} 比较的值必须是数字，传入：{val_str}")

                    if not isinstance(actual_value, (int, float)):
                        raise AssertionError(
                            f"字段 {key} 不是数字，无法比较，实际：{actual_value}")

                    if op == "lt":
                        assert actual_value < val, f"断言失败：{
                            key} 应小于 {val}，实际 {actual_value}"
                    elif op == "gt":
                        assert actual_value > val, f"断言失败：{
                            key} 应大于 {val}，实际 {actual_value}"
                    elif op == "le":
                        assert actual_value <= val, f"断言失败：{
                            key} 应小于等于 {val}，实际 {actual_value}"
                    elif op == "ge":
                        assert actual_value >= val, f"断言失败：{
                            key} 应大于等于 {val}，实际 {actual_value}"
                    elif op == "eq":
                        assert actual_value == val, f"断言失败：{
                            key} 应等于 {val}，实际 {actual_value}"
                else:
                    assert actual_value == expect_value, f"业务断言失败！字段：{
                        key}，预期：{expect_value}，实际：{actual_value}"

    # 8. 提取变量存入全局变量池
    if "extract" in case_item:
        if res_json is None:
            raise RuntimeError("无法提取变量：响应体不是合法JSON格式")
        for var_name, json_path in case_item["extract"].items():
            expr = parse(json_path)
            matches = [match.value for match in expr.find(res_json)]
            if not matches:
                raise RuntimeError(f"提取变量 [{var_name}] 失败，路径 {
                                   json_path} 未匹配到数据")
            var_pool[var_name] = matches[0]
