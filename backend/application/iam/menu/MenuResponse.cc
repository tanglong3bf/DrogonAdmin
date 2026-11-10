#include "MenuResponse.h"

#include <drogon/HttpResponse.h>

using namespace std;

Json::Value toJsonArray(const vector<MenuResponse> &data)
{
    Json::Value array(Json::arrayValue);
    for (const auto &item : data)
    {
        array.append(item.toJson());
    }
    return array;
}

namespace drogon
{
template <>
HttpResponsePtr toResponse(const vector<MenuResponse> &response)
{
    Json::Value json;
    json["data"] = toJsonArray(response);
    return HttpResponse::newHttpJsonResponse(json);
}
};  // namespace drogon

Json::Value MenuResponse::toJson() const
{
    Json::Value result;
    result["menu_id"] = menuId_;
    result["sort_num"] = sortNum_;
    result["version"] = version_;
    result["name"] = name_;
    if (parentId_)
    {
        result["parent_id"] = *parentId_;
    }
    if (path_)
    {
        result["path"] = *path_;
    }
    if (icon_)
    {
        result["icon"] = *icon_;
    }
    result["type"] = type_;
    if (component_)
    {
        result["component"] = *component_;
    }
    if (children_.size() > 0)
    {
        result["children"] = toJsonArray(children_);
    }
    return result;
}

MenuResponse::MenuResponse(const Menu &menu)
    : menuId_{*menu.menuId()},
      sortNum_{menu.sortNum()},
      version_{menu.version()},
      parentId_{menu.parentId()},
      name_{menu.name()},
      path_{menu.path()},
      icon_{menu.icon()},
      type_{static_cast<short>(menu.type())},
      component_{menu.component()}
{
}

void MenuResponse::addChild(MenuResponse &child)
{
    assert(child.parentId_ == menuId_ || (!child.parentId_ && !menuId_));
    if (!isChildIdExist(child.menuId_))
    {
        children_.emplace_back(child);
        child.parent_ = this;
    }
}

bool MenuResponse::isChildIdExist(const int32_t id) noexcept
{
    for (const auto &child : children_)
    {
        if (child.menuId_ == id)
        {
            return true;
        }
    }
    return false;
}
