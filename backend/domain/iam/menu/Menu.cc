#include "Menu.h"

using namespace std;
using namespace trantor;
using namespace drogon_model::drogon_admin_db;
using namespace drogon_admin::util;

Menu::Menu(string_view name, const int32_t sortNum, const MenuType type)
    : name_{name}, sortNum_{sortNum}, type_{type}, version_{0}
{
}

Menu::Menu(string_view name,
           const int32_t sortNum,
           const MenuType type,
           const int32_t createdBy)
    : AuditableEntity{AUDITABLE_INIT},
      name_{name},
      sortNum_{sortNum},
      type_{type},
      version_{0}
{
}

Menu::Menu(const SysMenu &model)
    : AuditableEntity{AUDITABLE_INIT_BY_MODEL},
      OPT_INIT(menuId_, MenuId),
      OPT_OV_INIT(int32_t, actionId_, ActionId),
      OPT_INIT(parentId_, ParentId),
      OPT_INIT(path_, Path),
      OPT_INIT(icon_, Icon),
      INIT(name_, Name),
      INIT(sortNum_, SortNum),
      OPT_INIT(component_, Component),
      ENUM_INIT(MenuType, type_, Type),
      INIT(version_, Version)
{
}

Menu::operator SysMenu() const
{
    SysMenu model;
    SET_OPT(menuId_, MenuId);
    SET_OPT(actionId_, ActionId);
    SET_OPT(parentId_, ParentId);
    SET_OPT(path_, Path);
    SET_OPT(icon_, Icon);
    SET_VAL(name_, Name);
    SET_VAL(sortNum_, SortNum);
    SET_OPT(component_, Component);
    SET_VAL_CAST(short, type_, Type);
    SET_VAL(version_, Version);
    SET_OPT(createdBy(), CreatedBy);
    SET_OPT(createdTime(), CreatedTime);
    SET_OPT(updatedBy(), UpdatedBy);
    SET_OPT(updatedTime(), UpdatedTime);
    SET_OPT(deletedBy(), DeletedBy);
    SET_OPT(deletedTime(), DeletedTime);
    return model;
}

void Menu::setParentId(const optional<int32_t> parentId)
{
    if (parentId)
    {
        parentId_ = parentId;
    }
}

void Menu::setSortNum(const int32_t sortNum)
{
    sortNum_ = sortNum;
}

void Menu::constructOptionalFields(const optional<string> &icon,
                                   const optional<string> &path,
                                   const optional<string> &component)
{
    icon_ = icon;
    path_ = path;
    component_ = component;
}

void Menu::updateBasicInfo(const optional<string> name,
                           const NullableValue<int32_t> &parentId,
                           const int32_t sortNum,
                           const NullableValue<string> &icon,
                           const NullableValue<int32_t> &actionId,
                           const int32_t updatedBy)
{
    bool isUpdated = false;
    if (name && name_ != *name)
    {
        name_ = *name;
        isUpdated = true;
    }
    if (parentId && parentId_ != *parentId)
    {
        parentId_ = *parentId;
        isUpdated = true;
    }
    else if (parentId.isNull() && parentId_ != std::nullopt)
    {
        parentId_ = std::nullopt;
        isUpdated = true;
    }
    if (sortNum_ != sortNum)
    {
        sortNum_ = sortNum;
        isUpdated = true;
    }
    if (icon && icon_ != *icon)
    {
        icon_ = *icon;
        isUpdated = true;
    }
    else if (icon.isNull() && icon_ != std::nullopt)
    {
        icon_ = std::nullopt;
        isUpdated = true;
    }
    if (actionId && actionId_ != *actionId)
    {
        actionId_ = *actionId;
        isUpdated = true;
    }
    else if (actionId.isNull() && actionId_ != std::nullopt)
    {
        actionId_ = std::nullopt;
        isUpdated = true;
    }
    if (isUpdated)
    {
        markUpdatedBy(updatedBy);
        markUpdated();
    }
}

void Menu::updateTypeAndAccess(const optional<MenuType> type,
                               const NullableValue<string> &path,
                               const NullableValue<string> &component,
                               const int32_t updatedBy)
{
    // 未传类型时沿用当前类型，path、component 未传时沿用当前值
    const MenuType newType = type.value_or(type_);

    optional<string> newPath = path_;
    if (!path.isAbsent())
    {
        newPath = path.hasValue() ? optional<string>{path.value()} : nullopt;
    }

    optional<string> newComponent = component_;
    if (!component.isAbsent())
    {
        newComponent = component.hasValue()
                           ? optional<string>{component.value()}
                           : nullopt;
    }

    switch (newType)
    {
        case MenuType::Directory:
            // 目录类型不需要访问路径和组件
            newPath = nullopt;
            newComponent = nullopt;
            break;

        case MenuType::Page:
            // 页面类型必须有访问路径和组件
            if (!newPath)
            {
                throw BusinessException{"页面类型的菜单必须设置访问路径"};
            }
            if (!newComponent)
            {
                throw BusinessException{"页面类型的菜单必须设置组件"};
            }
            break;

        case MenuType::OutLink:
            // 外链类型必须有访问路径，不需要组件
            if (!newPath)
            {
                throw BusinessException{"外链类型的菜单必须设置访问路径"};
            }
            newComponent = nullopt;
            break;
    }

    bool isUpdated = false;
    if (type && type_ != *type)
    {
        type_ = *type;
        isUpdated = true;
    }
    if (newPath != path_)
    {
        path_ = newPath;
        isUpdated = true;
    }
    if (newComponent != component_)
    {
        component_ = newComponent;
        isUpdated = true;
    }
    if (isUpdated)
    {
        markUpdatedBy(updatedBy);
        markUpdated();
    }
}

void Menu::remove(const int32_t deletedBy)
{
    markDeletedBy(deletedBy);
    markDeleted();
}
