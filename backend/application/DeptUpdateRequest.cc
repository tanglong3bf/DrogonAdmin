#include <drogon/HttpRequest.h>
#include "common/util/ParamGetter.hpp"
#include "DeptUpdateRequest.h"

using namespace std;
using namespace drogon_admin::util;

namespace drogon
{
template <>
DeptUpdateRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    DeptUpdateRequest request;
    request.setByJson(*jsonPtr);
    return request;
}
};  // namespace drogon

void DeptUpdateRequest::setByJson(const Json::Value &json)
{
    name_ = getParam<string, true>(json, "name", {1, -1});
}
