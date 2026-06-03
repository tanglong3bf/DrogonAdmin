#include "UserUpdateRequest.h"

#include "common/util/ParamGetter.hpp"
#include <drogon/HttpRequest.h>

using namespace std;
using namespace drogon_admin::util;

namespace drogon
{
template <>
UserUpdateRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    UserUpdateRequest request(*jsonPtr);
    return request;
}
};  // namespace drogon

UserUpdateRequest::UserUpdateRequest(const Json::Value &json)
{
    nickname_ = getParam<std::string>(json, "nickname");
    auto sexInt = getParam<int32_t>(json, "sex", {0, 2});
    if (sexInt)
    {
        sex_ = static_cast<Sex>(*sexInt);
    }

    deptId_ = getParam<int32_t>(json, "dept_id", {0, -1});
    phoneNumber_ = getParam<std::string>(json, "phone_number", {11, 11});
    email_ = getParam<std::string>(json, "email");

    auto statusInt = getParam<int32_t>(json, "status", {0, 2});
    if (statusInt)
    {
        status_ = static_cast<Status>(*statusInt);
    }

    roleIds_ = getParam<std::vector<int32_t>>(json, "role_ids", {0, -1});
}
