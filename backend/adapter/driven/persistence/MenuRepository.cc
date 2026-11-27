#include "domain/iam/menu/MenuRepository.h"

#include "common/exception/BusinessException.h"
#include "common/framework/domain/ChangeableEntity.h"
#include "common/util/rangesUtils.hpp"
#include <drogon/orm/CoroMapper.h>
#include <drogon/orm/Criteria.h>
#include <ranges>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_admin;
using namespace drogon_model::drogon_admin_db;
using namespace tl::sql;

Task<std::optional<Menu>> MenuRepository::getById(
    const std::int32_t menuId) const
{
    Criteria criteria{SysMenu::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysMenu::Cols::_menu_id, menuId};

    try
    {
        const auto sysMenu = co_await menuMapper().findOne(criteria);
        co_return Menu{sysMenu};
    }
    catch (const orm::UnexpectedRows & /*ignore*/)
    {
        co_return std::nullopt;
    }
}

Task<std::size_t> MenuRepository::countNameByParentId(
    const string &name,
    const optional<std::int32_t> &parentId) const
{
    Criteria criteria{SysMenu::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysMenu::Cols::_name, name};
    if (parentId)
    {
        criteria = criteria && Criteria{SysMenu::Cols::_parent_id, *parentId};
    }
    else
    {
        criteria = criteria &&
                   Criteria{SysMenu::Cols::_parent_id, CompareOperator::IsNull};
    }

    co_return co_await menuMapper().count(criteria);
}

Task<> MenuRepository::save(Menu &menu) const
{
    switch (menu.changingStatus())
    {
        case ChangingStatus::NEW:
        {
            auto trans =
                co_await drogon::app().getDbClient()->newTransactionCoro();
            const auto sysMenu = static_cast<SysMenu>(menu);
            co_await menuMapper(trans).insert(sysMenu);
            break;
        }
        case ChangingStatus::DELETED:
        {
            const auto result =
                co_await drogon::app().getDbClient()->execSqlCoro(
                    "UPDATE sys_menu SET deleted_by = $1, deleted_time = $2, "
                    "version = version + 1 WHERE menu_id = $3 AND version = $4",
                    *menu.deletedBy(),
                    *menu.deletedTime(),
                    *menu.menuId(),
                    menu.version());
            if (result.affectedRows() == 0)
            {
                throw BusinessException{"删除期间数据发生变化，删除失败"};
            }
            break;
        }
        case ChangingStatus::UPDATED:
        {
            const auto trans =
                co_await drogon::app().getDbClient()->newTransactionCoro();
            const auto sysMenu = static_cast<SysMenu>(menu);
            Json::Value json = sysMenu.toJson();
            string sql =
                sqlGenerator()->getSql("update_menu", {{"data", json}});
            const auto result = co_await trans->execSqlCoro(sql);
            if (result.affectedRows() != 1)
            {
                trans->rollback();
                throw BusinessException{"更新期间数据发生变化，更新失败"};
            }
            break;
        }
        default:
            break;
    }
}

Task<std::optional<std::int32_t>> MenuRepository::getMaxSubMenuSortNum(
    const std::optional<std::int32_t> parentId,
    const DbClientPtr &dbClient) const
{
    ParamList paramList;
    if (parentId)
    {
        paramList = {{"parent_id", static_cast<int>(*parentId)}};
    }

    const auto sql =
        sqlGenerator()->getSql("get_max_sub_menu_sort_num", paramList);

    const auto dbResult = co_await dbClient->execSqlCoro(sql);
    if (dbResult[0][0].isNull())
    {
        co_return std::nullopt;
    }
    co_return dbResult[0][0].as<std::int32_t>();
}

Task<std::size_t> MenuRepository::countByActionIds(
    const vector<std::int32_t> &actionIds) const
{
    Criteria criteria{SysMenu::Cols::_deleted_by, CompareOperator::IsNull};
    criteria =
        criteria &&
        Criteria{SysMenu::Cols::_action_id, CompareOperator::In, actionIds};
    co_return co_await menuMapper().count(criteria);
}

Task<size_t> MenuRepository::countSubMenu(const int32_t menuId) const
{
    Criteria criteria{SysMenu::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysMenu::Cols::_parent_id, menuId};
    co_return co_await menuMapper().count(criteria);
}

Task<bool> MenuRepository::isAncestorOrSelf(const int32_t menuId,
                                            const int32_t parentId,
                                            const DbClientPtr &dbClient) const
{
    const auto sql =
        sqlGenerator()->getSql("count_menu_ancestor",
                               {{"menu_id", menuId}, {"parent_id", parentId}});
    const auto dbResult = co_await dbClient->execSqlCoro(sql);
    co_return dbResult[0][0].as<std::int64_t>() > 0;
}

Task<vector<Menu>> MenuRepository::getByParentId(
    const optional<std::int32_t> &parentId) const
{
    Criteria criteria{SysMenu::Cols::_deleted_by, CompareOperator::IsNull};
    criteria =
        criteria && (parentId ? Criteria{SysMenu::Cols::_parent_id, *parentId}
                              : Criteria{SysMenu::Cols::_parent_id,
                                         CompareOperator::IsNull});
    const auto menuInDb = co_await menuMapper().findBy(criteria);
    co_return menuInDb |
        views::transform([](const auto &data) { return Menu{data}; }) |
        ranges_utils::to<vector>();
}

Task<> MenuRepository::multiSave(const vector<Menu> &menus,
                                 const vector<int32_t> &versions,
                                 const DbClientPtr &dbClient) const
{
    assert(menus.size() == versions.size());
    // 暂只考虑批量更新
    vector<SysMenu> toUpdate;
    toUpdate.reserve(menus.size());
    for (const auto &menu : menus)
    {
        if (menu.changingStatus() == ChangingStatus::UPDATED)
        {
            toUpdate.push_back(static_cast<SysMenu>(menu));
        }
    }
    if (toUpdate.size() == 0)
    {
        LOG_WARN << "multiSave被调用，但没有需要更新的数据，请检查代码逻辑";
        co_return;
    }

    Json::Value dataList;
    for (const auto &menu : toUpdate)
    {
        dataList.append(menu.toJson());
    }
    Json::Value versionList;
    for (const auto &version : versions)
    {
        versionList.append(version);
    }

    const auto sql =
        sqlGenerator()->getSql("multi_update_menu",
                               ParamList{{"data_list", dataList},
                                         {"version_list", versionList}});

    auto trans = co_await dbClient->newTransactionCoro();
    const auto result = co_await trans->execSqlCoro(sql);
    if (result.affectedRows() < menus.size())
    {
        trans->rollback();
        throw BusinessException{"更新期间数据发生变化，更新失败"};
    }
}

inline SqlGenerator *MenuRepository::sqlGenerator()
{
    static const auto sqlGenerator_ = app().getPlugin<SqlGenerator>();
    return sqlGenerator_;
}

inline CoroMapper<SysMenu> MenuRepository::menuMapper(
    const DbClientPtr &dbClient)
{
    return CoroMapper<SysMenu>{dbClient};
}
