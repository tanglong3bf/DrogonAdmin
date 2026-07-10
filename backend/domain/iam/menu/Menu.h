#pragma once

#include <vector>
#include "common/framework/domain/AuditableEntity.h"
#include "common/framework/domain/ChangeableEntity.h"

enum class MenuType : int16_t
{
    Menu,    ///< 菜单
    Page,    ///< 页面
    OutLink  ///< 外链
};

class Menu : public AuditableEntity, public ChangeableEntity
{
  private:
    // 新增时没有id
    std::optional<std::int32_t> menuId_;    ///< 菜单id
    std::optional<int32_t> functionId_;     ///< 访问菜单需要的功能id
    std::optional<std::int32_t> parentId_;  ///< 父菜单
    std::optional<std::string> path_;       ///< 访问路径，仅在Menu时为空
    std::optional<std::string> icon_;       ///< 图标、可选
    std::string name;                       ///< 菜单名称
    std::int32_t sortNum_;                  ///< 排序
    std::optional<std::string> component_;  ///< 组件路径，仅在Page时有值
    MenuType type_;                         ///< 菜单类型
    std::vector<Menu> children_;            ///< 子菜单
};
