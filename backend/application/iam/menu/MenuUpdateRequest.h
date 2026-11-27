#pragma once

#include "common/util/Utilities.hpp"
#include "domain/iam/menu/Menu.h"
#include <jsoncpp/json/value.h>
#include <optional>

/**
 * @brief 更新菜单项请求参数
 */
struct MenuUpdateRequest
{
    MenuUpdateRequest() = default;
    explicit MenuUpdateRequest(const Json::Value &json);

    GETTER(name)
    GETTER(type)
    GETTER(parentId)
    GETTER(path)
    GETTER(component)
    GETTER(icon)
    GETTER(actionId)
    GETTER(version)

  private:
    std::optional<std::string> name_;                           ///< 菜单名称
    std::optional<MenuType> type_;                              ///< 菜单类型
    drogon_admin::util::NullableValue<std::int32_t> parentId_;  ///< 父菜单ID
    drogon_admin::util::NullableValue<std::string> path_;       ///< 访问路径
    drogon_admin::util::NullableValue<std::string> component_;  ///< 组件路径
    drogon_admin::util::NullableValue<std::string> icon_;       ///< 图标
    drogon_admin::util::NullableValue<int32_t> actionId_;       ///< 所需功能ID
    std::int32_t version_;  ///< 乐观锁版本号
};
