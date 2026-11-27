#include "MenuSortRequest.h"

#include "common/util/ParamGetter.hpp"
#include "common/util/rangesUtils.hpp"
#include "common/exception/BusinessException.h"
#include <drogon/HttpRequest.h>
#include <ranges>

using namespace std;
using namespace drogon_admin;
using namespace drogon_admin::util;

namespace drogon
{
template <>
MenuSortRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    MenuSortRequest request(*jsonPtr);
    return request;
}
};  // namespace drogon

MenuSortRequest::MenuSortRequest(const Json::Value &json)
{
    parentId_ = getParam<int32_t, false>(json, "parent_id", {1, -1});
    if (json.isMember("menus") && json["menus"].isArray())
    {
        for (const auto &jsonItem : json["menus"])
        {
            MenuSortItem item;
            item.menuId = getParam<int32_t, true>(jsonItem, "menu_id");
            item.version = getParam<int32_t, true>(jsonItem, "version");
            menus_.emplace_back(item);
        }
    }
    if (menus_.size() == 0)
    {
        throw BusinessException("缺少必备参数：menus");
    }
}

vector<int32_t> MenuSortRequest::menuIds() const
{
    return menus_ |
           views::transform([](const auto &item) { return item.menuId; }) |
           ranges_utils::to<vector>();
}
