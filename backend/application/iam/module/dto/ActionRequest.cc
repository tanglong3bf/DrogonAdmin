#include "ActionRequest.h"

#include "common/util/ParamGetter.hpp"
#include "common/exception/BusinessException.h"
#include <drogon/HttpRequest.h>

using namespace std;
using namespace drogon_admin::util;

namespace drogon
{
template <>
ActionRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    ActionRequest request(*jsonPtr);
    return request;
}

}  // namespace drogon

ActionRequest::ActionRequest(const Json::Value &json)
{
    LOG_DEBUG << json.toStyledString();
    actionId_ = getParam<std::int64_t, true>(json, "action_id");
    name_ = getParam<std::string, true>(json, "name", {1, -1});
    code_ = getParam<std::string, true>(json, "code", {1, -1});
    moduleId_ = getParam<std::int32_t, true>(json, "module_id", {1, -1});
    hasDataPermission_ = getParam<bool, true>(json, "has_data_permission");
    sortNum_ = getParam<std::int32_t, true>(json, "sort_num", {0, -1});
}
