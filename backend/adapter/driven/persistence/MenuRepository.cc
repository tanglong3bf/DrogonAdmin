#include "domain/iam/menu/MenuRepository.h"

#include "common/framework/domain/ChangeableEntity.h"
#include <drogon/orm/CoroMapper.h>
#include <drogon/orm/Criteria.h>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
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
