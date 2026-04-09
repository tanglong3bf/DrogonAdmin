#include "UserCreateRequest.h"

#include <drogon/HttpRequest.h>
#include "common/util/ParamGetter.hpp"
#include "common/exception/BusinessException.h"

using namespace std;
using namespace drogon_admin::util;

namespace drogon
{
template <>
UserCreateRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    UserCreateRequest request;
    request.setByJson(*jsonPtr);
    return request;
}
};  // namespace drogon

void UserCreateRequest::setByJson(const Json::Value &json)
{
    username_ = getParam<string, true>(json, "username");
    nickname_ = getParam<string, true>(json, "nickname");

    const auto sex = getParam<int8_t, true>(json, "sex");
    sex_ = static_cast<Sex>(sex);

    deptId_ = getParam<int32_t, true>(json, "dept_id");
    phoneNumber_ = getParam<string>(json, "phone_number");
    email_ = getParam<string>(json, "email");

    const auto status = getParam<int8_t, true>(json, "status");
    status_ = static_cast<Status>(status);

    roleIds_ = getParam<vector<int32_t>>(json, "role_ids");
}
