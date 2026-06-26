#include "ChangePasswordRequest.h"

#include "common/exception/BusinessException.h"
#include "common/util/ParamGetter.hpp"
#include <drogon/HttpRequest.h>

using namespace std;
using namespace drogon_admin::util;

namespace drogon
{
template <>
ChangePasswordRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    ChangePasswordRequest request(*jsonPtr);
    return request;
}
};  // namespace drogon

ChangePasswordRequest::ChangePasswordRequest(const Json::Value &json)
{
    oldPassword_ = getParam<string, true>(json, "oldPassword", {6, 20});
    newPassword_ = getParam<string, true>(json, "newPassword", {6, 20});
}
