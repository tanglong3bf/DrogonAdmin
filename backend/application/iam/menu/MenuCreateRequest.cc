#include "MenuCreateRequest.h"

#include "common/exception/BusinessException.h"
#include "common/util/ParamGetter.hpp"
#include <drogon/HttpRequest.h>

using namespace std;
using namespace drogon_admin::util;

namespace drogon
{
template <>
MenuCreateRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    MenuCreateRequest request(*jsonPtr);
    return request;
}
};  // namespace drogon

MenuCreateRequest::MenuCreateRequest(const Json::Value &json)
{
    name_ = getParam<string, true>(json, "name", {2, 50});
    type_ = static_cast<MenuType>(
        getParam<std::int16_t, true>(json, "type", {0, 2}));
    parentId_ = getParam<int32_t>(json, "parent_id", {1, -1});
    icon_ = getParam<string>(json, "icon", {1, -1});

    switch (type_)
    {
        case MenuType::Directory:
            // 目录类型：不需要path和component
            path_ = std::nullopt;
            component_ = std::nullopt;
            break;

        case MenuType::Page:
            // 页面类型：path和component必填
            path_ = getParam<string, true>(json, "path", {1, 50});
            component_ = getParam<string, true>(json, "component", {1, 50});
            break;

        case MenuType::OutLink:
            // 外链类型：path必填，component不需要
            path_ = getParam<string, true>(json, "path", {1, 50});
            component_ = std::nullopt;
            break;
    }
}
