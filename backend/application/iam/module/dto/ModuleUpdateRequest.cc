#include "ModuleUpdateRequest.h"

#include <drogon/HttpRequest.h>

using namespace std;
using namespace drogon_admin::util;

namespace drogon
{
template <>
ModuleUpdateRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    ModuleUpdateRequest request(*jsonPtr);
    return request;
}
};  // namespace drogon

ModuleUpdateRequest::ModuleUpdateRequest(const Json::Value &json)
{
    name_ = getParam<string>(json, "name", {1, -1});
    description_ = getParam<string, false, true>(json, "description", {1, -1});
    if (!name_ && description_.isAbsent())
    {
        throw BusinessException("请至少指定一个需要更新的字段");
    }
    version_ = getParam<int32_t, true>(json, "version", {0, -1});
}
