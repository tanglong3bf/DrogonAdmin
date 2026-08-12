#include "ModuleCreateRequest.h"

#include "common/util/ParamGetter.hpp"
#include "common/exception/BusinessException.h"
#include <drogon/HttpRequest.h>

using namespace std;
using namespace drogon_admin::util;

namespace drogon
{
template <>
ModuleCreateRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    ModuleCreateRequest request(*jsonPtr);
    return request;
}
};  // namespace drogon

ModuleCreateRequest::ModuleCreateRequest(const Json::Value &json)
{
    name_ = getParam<std::string, true>(json, "name", {1, -1});
    description_ = getParam<std::string>(json, "description", {1, -1});
    parentId_ = getParam<std::int32_t>(json, "parent_id", {1, -1});
}
