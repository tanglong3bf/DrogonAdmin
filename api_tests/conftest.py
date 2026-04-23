def pytest_itemcollected(item):
    # 修复 pytest -vv 中文参数显示 \uXXXX 问题，同时保留完整格式
    if '\\u' in item._nodeid:
        # 只转义中文部分，不破坏 nodeid 格式
        item._nodeid = item._nodeid.encode('utf-8').decode('unicode_escape')
