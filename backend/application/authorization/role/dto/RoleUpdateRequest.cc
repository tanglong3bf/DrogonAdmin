#include "RoleUpdateRequest.h"

#include "common/util/ParamGetter.hpp"
#include <drogon/HttpRequest.h>

using namespace std;
using namespace drogon_admin::util;

namespace drogon
{
template <>
RoleUpdateRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    RoleUpdateRequest request;
    request.setByJson(*jsonPtr);
    return request;
}
};  // namespace drogon

void RoleUpdateRequest::setByJson(const Json::Value &json)
{
    name_ = getParam<string>(json, "name", {6, 50});
    code_ = getParam<string>(json, "code", {2, 32});
    description_ = getParam<string, false, true>(json, "description", {1, -1});
    const auto quotaTypeInt =
        getParam<std::int16_t>(json, "quota_type", {0, 2});
    if (quotaTypeInt)
    {
        quotaType_ = static_cast<QuotaType>(*quotaTypeInt);
    }
    userQuota_ =
        getParam<std::int32_t, false, true>(json, "user_quota", {1, -1});
    const auto relationTypeInt = getParam<std::int16_t>(json, "relation_type");
    if (relationTypeInt)
    {
        relationType_ = static_cast<RelationType>(*relationTypeInt);
    }
    deptIds_ = getParam<vector<std::int32_t>>(json, "dept_ids");
}
