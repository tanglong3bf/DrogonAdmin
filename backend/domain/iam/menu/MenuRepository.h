#pragma once

#include "Menu.h"
#include "domain/models/SysMenu.h"
#include "common/framework/DrAdminObject.hpp"
#include "SqlGenerator/src/SqlGenerator.h"
#include <drogon/HttpAppFramework.h>
#include <drogon/utils/coroutine.h>

class MenuRepository : public DrAdminObject<MenuRepository>
{
    using SqlGenerator = tl::sql::SqlGenerator;
    using SysMenu = drogon_model::drogon_admin_db::SysMenu;
    using SysMenuMapper = drogon::orm::CoroMapper<SysMenu>;
    using DbClientPtr = drogon::orm::DbClientPtr;

  public:
    /**
     * @brief 保存菜单实体（新建、更新、删除）
     */
    drogon::Task<> save(Menu &menu) const;

    /**
     * @brief 获取指定父菜单下最大的 sort_num
     */
    drogon::Task<std::optional<std::int32_t>> getMaxSubMenuSortNum(
        const std::optional<std::int32_t> parentId,
        const DbClientPtr & = drogon::app().getDbClient()) const;

    /**
     * @brief 根据 id 获取菜单
     */
    drogon::Task<std::optional<Menu>> getById(const std::int32_t menuId) const;

    /**
     * @brief 获取指定父菜单下的所有子菜单
     */
    drogon::Task<std::vector<Menu>> getByParentId(
        const std::optional<std::int32_t> &parentId) const;

    /**
     * @brief 批量存储菜单
     */
    drogon::Task<> multiSave(
        const std::vector<Menu> &menus,
        const std::vector<std::int32_t> &versions,
        const DbClientPtr & = drogon::app().getDbClient()) const;

    /**
     * @brief 统计指定父菜单下同名子菜单数量
     */
    drogon::Task<std::size_t> countNameByParentId(
        const std::string &name,
        const std::optional<std::int32_t> &parentId) const;

    drogon::Task<std::size_t> countByActionIds(
        const std::vector<std::int32_t> &actionIds) const;

    /**
     * @brief 统计指定菜单的子菜单数量
     */
    drogon::Task<std::size_t> countSubMenu(std::int32_t menuId) const;

    /**
     * @brief 判断 menuId 是否为目标菜单自身或其祖先
     */
    drogon::Task<bool> isAncestorOrSelf(
        std::int32_t menuId,
        std::int32_t parentId,
        const DbClientPtr & = drogon::app().getDbClient()) const;

  private:
    static SqlGenerator *sqlGenerator();
    static SysMenuMapper menuMapper(
        const DbClientPtr &dbClient = drogon::app().getDbClient());
};

using MenuRepositoryPtr = std::shared_ptr<MenuRepository>;
