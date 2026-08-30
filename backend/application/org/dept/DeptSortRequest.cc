#include "DeptSortRequest.h"

#include "common/util/ParamGetter.hpp"
#include "common/util/rangesUtils.hpp"
#include "common/exception/BusinessException.h"
#include <drogon/HttpRequest.h>
#include <ranges>

using namespace std;
using namespace drogon_admin;
using namespace drogon_admin::util;

namespace drogon
{
template <>
DeptSortRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    DeptSortRequest request(*jsonPtr);
    return request;
}
};  // namespace drogon

DeptSortRequest::DeptSortRequest(const Json::Value &json)
{
    parentId_ = getParam<int32_t, false>(json, "parent_id", {1, -1});
    if (json.isMember("depts") && json["depts"].isArray())
    {
        size_t i = 0;
        for (const auto &jsonItem : json["depts"])
        {
            DeptSortItem item;
            item.deptId = getParam<int32_t, true>(jsonItem, "dept_id");
            item.version = getParam<int32_t, true>(jsonItem, "version");
            depts_.emplace_back(item);
        }
    }
    if (depts_.size() == 0)
    {
        throw BusinessException("缺少必备参数：depts");
    }
}

vector<int32_t> DeptSortRequest::deptIds() const
{
    return depts_ |
           views::transform([](const auto &item) { return item.deptId; }) |
           ranges_utils::to<vector>();
}
