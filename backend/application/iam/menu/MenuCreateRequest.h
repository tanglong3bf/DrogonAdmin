#pragma once

#include "domain/iam/menu/Menu.h"
#include "common/util/Utilities.hpp"
#include <jsoncpp/json/value.h>
#include <optional>

/**
 * @brief 新增菜单项请求参数
 */
struct MenuCreateRequest
{
    MenuCreateRequest() = default;
    explicit MenuCreateRequest(const Json::Value &json);

    GETTER_STR_VIEW(name)
    GETTER(type)
    GETTER(parentId)
    GETTER(path)
    GETTER(icon)
    GETTER(component)

  private:
    std::string name_;                      ///< 菜单名称
    MenuType type_;                         ///< 菜单类型
    std::optional<std::int32_t> parentId_;  ///< 父菜单ID
    std::optional<std::string> path_;       ///< 访问路径
    std::optional<std::string> icon_;       ///< 图标
    std::optional<std::string> component_;  ///< 组件路径
};
