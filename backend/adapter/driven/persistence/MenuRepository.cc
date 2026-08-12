#include "domain/iam/menu/MenuRepository.h"

#include <drogon/orm/CoroMapper.h>
#include <drogon/orm/Criteria.h>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::drogon_admin_db;
using namespace tl::sql;

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
