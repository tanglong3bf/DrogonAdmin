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
    name_ = getParam<string>(json, "name");
    code_ = getParam<string>(json, "code");
    description_ = getParam<string>(json, "description");
    const auto quotaTypeInt = getParam<int32_t>(json, "quota_type");
    if (quotaTypeInt)
    {
        quotaType_ = static_cast<QuotaType>(*quotaTypeInt);
    }
    userQuota_ = getParam<int32_t>(json, "user_quota");
    const auto relationTypeInt = getParam<int32_t>(json, "relation_type");
    if (relationTypeInt)
    {
        relationType_ = static_cast<RelationType>(*relationTypeInt);
    }
    deptIds_ = getParam<vector<int32_t>>(json, "dept_ids");
}
