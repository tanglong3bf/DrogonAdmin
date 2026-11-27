#include "MenuUpdateRequest.h"

#include "common/util/ParamGetter.hpp"
#include <drogon/HttpRequest.h>

using namespace std;
using namespace drogon_admin::util;

namespace drogon
{
template <>
MenuUpdateRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    MenuUpdateRequest request{*jsonPtr};
    return request;
}
};  // namespace drogon

MenuUpdateRequest::MenuUpdateRequest(const Json::Value &json)
{
    name_ = getParam<string>(json, "name", {2, 20});
    icon_ = getParam<string, false, true>(json, "icon", {1, -1});
    path_ = getParam<string, false, true>(json, "path", {1, -1});
    component_ = getParam<string, false, true>(json, "component", {1, -1});

    const auto typeInt = getParam<int16_t>(json, "type", {0, 2});
    if (typeInt)
    {
        type_ = static_cast<MenuType>(*typeInt);
    }

    parentId_ = getParam<int32_t, false, true>(json, "parent_id", {1, -1});
    actionId_ = getParam<int32_t, false, true>(json, "action_id", {1, -1});
    version_ = getParam<int32_t, true>(json, "version", {0, -1});
}
