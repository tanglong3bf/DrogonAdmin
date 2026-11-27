#pragma once

#include "common/util/Utilities.hpp"
#include <jsoncpp/json/value.h>
#include <optional>

struct MenuSortItem
{
    std::int32_t menuId;
    std::int32_t version;
};

/**
 * @brief 菜单排序请求
 */
struct MenuSortRequest
{
    MenuSortRequest() = default;
    MenuSortRequest(const Json::Value &json);

    GETTER(parentId)
    GETTER(menus)

    std::vector<std::int32_t> menuIds() const;

  private:
    std::optional<std::int32_t> parentId_;  ///< 父菜单id
    std::vector<MenuSortItem> menus_;       ///< 菜单排序项
};
