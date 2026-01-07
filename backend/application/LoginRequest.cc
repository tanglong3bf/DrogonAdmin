#include "LoginRequest.h"
#include <drogon/HttpRequest.h>
#include "common/exception/BusinessException.h"
#include "common/util/ParamGetter.hpp"

using namespace drogon_admin::util;

namespace drogon
{
template <>
LoginRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json",
                                k400BadRequest);
    }
    LoginRequest request;
    request.setByJson(*jsonPtr);
    return request;
}
};  // namespace drogon

void LoginRequest::setByJson(const Json::Value &json)
{
    username_ = getParam<std::string, true>(json, "username", {6, 20});
    password_ = getParam<std::string, true>(json, "password", {6, 20});
}
