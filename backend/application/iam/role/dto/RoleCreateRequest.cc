#include "RoleCreateRequest.h"

#include "common/util/ParamGetter.hpp"
#include <drogon/HttpRequest.h>

using namespace std;
using namespace drogon_admin::util;

namespace drogon
{
template <>
RoleCreateRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    RoleCreateRequest request(*jsonPtr);
    return request;
}
};  // namespace drogon

RoleCreateRequest::RoleCreateRequest(const Json::Value &json)
{
    name_ = getParam<string, true>(json, "name", {2, 20});
    code_ = getParam<string, true>(json, "code", {2, 32});
    description_ = getParam<string>(json, "description", {1, -1});

    quotaType_ = static_cast<QuotaType>(
        getParam<std::int16_t, true>(json, "quota_type", {0, 2}));

    if (quotaType_ != QuotaType::Unlimited)
    {
        userQuota_ = getParam<std::int32_t, true>(json, "user_quota", {1, -1});
    }

    relationType_ = static_cast<RelationType>(
        getParam<std::int16_t, true>(json, "relation_type", {0, 2}));

    if (relationType_ != RelationType::All)
    {
        deptIds_ =
            getParam<vector<std::int32_t>, true>(json, "dept_ids", {1, -1});
    }
}
