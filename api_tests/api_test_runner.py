import pytest
import requests
import yaml
from jsonpath_ng import parse
from pathlib import Path
import psycopg2
from psycopg2 import OperationalError
# python3-jsonpath-ng
# python3-requests
# python3-pytest


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


def validate_schema(data, schema):
    """
    递归校验树形结构
    """
    # 校验必选字段
    for key in schema.get("required", []):
        assert key in data, f"缺失必选字段：{key}"

    # 校验字段类型
    for key, rule in schema.get("properties", {}).items():
        if key not in data:
            continue

        value = data[key]
        expected_type = rule["type"]

        # 类型校验
        if expected_type == "integer":
            assert isinstance(value, int), f"{key} 必须是数字"
        elif expected_type == "string":
            assert isinstance(value, str), f"{key} 必须是字符串"
        elif expected_type == "array":
            assert isinstance(value, list), f"{key} 必须是数组"
            # 数组子项递归校验
            if "item_schema" in rule:
                for item in value:
                    validate_schema(item, rule["item_schema"])
            elif "items" in rule:
                for item in value:
                    validate_schema(item, rule["items"])

        # 可空判断
        if value is None:
            assert rule.get("nullable", False), f"{key} 不允许为null"


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


datas: dict = read_yaml("cases/dept_test_case.yaml")
# 提取用例 name 做展示名
case_names = [case_item["name"] for case_item in datas["dept_cases"]]
dic = {}


@pytest.mark.parametrize("case_item", datas["dept_cases"], ids=case_names)
def test_dept_cases(case_item):
    # 部分接口需要提前准备数据
    if "sql_file" in case_item:
        run_sql_file(case_item["sql_file"])

    # 发起请求
    resp = requests.request(url=case_item["url"], method=case_item["method"],
                            json=case_item["json"], params=case_item["params"])

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
            schema = load_schema("schemas/" + schema_file)
            if isinstance(schema, dict) and "response" in schema:
                validate_schema(res_json, schema["response"])

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

    # 需要从响应体中提取变量
    if "extract" in case_item:
        for key, value in case_item["extract"].items():
            expr = parse(value)
            dic[key] = [
                match.value for match in expr.find(resp.json())][0]
