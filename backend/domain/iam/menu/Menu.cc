#include "Menu.h"

using namespace std;
using namespace trantor;
using namespace drogon_model::drogon_admin_db;

Menu::Menu(string_view name, const int32_t sortNum, const MenuType type)
    : name_{name}, version_{0}, sortNum_{sortNum}, type_{type}
{
}

Menu::Menu(string_view name,
           const int32_t sortNum,
           const MenuType type,
           const int32_t createdBy)
    : name_{name},
      version_{0},
      sortNum_{sortNum},
      type_{type},
      AuditableEntity{AUDITABLE_INIT}
{
}

Menu::Menu(const SysMenu &model)
    : OPT_INIT(menuId_, MenuId),
      INIT(version_, Version),
      OPT_OV_INIT(int32_t, actionId_, ActionId),
      OPT_INIT(parentId_, ParentId),
      OPT_INIT(path_, Path),
      OPT_INIT(icon_, Icon),
      INIT(name_, Name),
      INIT(sortNum_, SortNum),
      OPT_INIT(component_, Component),
      ENUM_INIT(MenuType, type_, Type),
      AuditableEntity{AUDITABLE_INIT_BY_MODEL}
{
}

Menu::operator SysMenu() const
{
    SysMenu model;
    SET_OPT(menuId_, MenuId);
    SET_VAL(version_, Version);
    SET_OPT(actionId_, ActionId);
    SET_OPT(parentId_, ParentId);
    SET_OPT(path_, Path);
    SET_OPT(icon_, Icon);
    SET_VAL(name_, Name);
    SET_VAL(sortNum_, SortNum);
    SET_OPT(component_, Component);
    SET_VAL_CAST(short, type_, Type);
    SET_OPT(createdBy(), CreatedBy);
    SET_OPT(createdTime(), CreatedTime);
    SET_OPT(updatedBy(), UpdatedBy);
    SET_OPT(updatedTime(), UpdatedTime);
    SET_OPT(deletedBy(), DeletedBy);
    SET_OPT(deletedTime(), DeletedTime);
    return model;
}

void Menu::setParentId(const std::optional<std::int32_t> parentId)
{
    if (parentId)
    {
        parentId_ = parentId;
    }
}

void Menu::constructOptionalFields(const std::optional<std::string> &icon,
                                   const std::optional<std::string> &path,
                                   const std::optional<std::string> &component)
{
    icon_ = icon;
    path_ = path;
    component_ = component;
}

void Menu::remove(const std::int32_t deletedBy)
{
    markDeletedBy(deletedBy);
    markDeleted();
}
