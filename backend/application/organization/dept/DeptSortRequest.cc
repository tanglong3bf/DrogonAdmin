#include "DeptSortRequest.h"
#include <drogon/HttpRequest.h>
#include <unordered_set>
#include "common/exception/BusinessException.h"
#include "common/util/ParamGetter.hpp"

using namespace std;
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
    DeptSortRequest request;
    request.setByJson(*jsonPtr);
    return request;
}
};  // namespace drogon

void DeptSortRequest::setByJson(const Json::Value &json)
{
    parentId_ = getParam<int32_t, false>(json, "parent_id", {1, -1});
    // deptIds_ 已保证不空
    deptIds_ = getParam<vector<int32_t>, true>(json, "dept_ids");

    // 进一步检查是否包含重复值
    const unordered_set<int32_t> idsSet{deptIds_.begin(), deptIds_.end()};
    if (idsSet.size() != deptIds_.size())
    {
        throw BusinessException("dept_ids参数包含重复值");
    }
}
