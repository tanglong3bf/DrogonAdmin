#include "domain/organization/UserRepository.h"

#include <drogon/HttpAppFramework.h>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::drogon_admin_db;
using namespace tl::sql;

Task<size_t> UserRepository::countByDept(const int32_t deptId) const
{
    Criteria criteria{SysUser::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysUser::Cols::_dept_id, deptId};

    co_return co_await userMapper().count(criteria);
}

inline SqlGenerator *UserRepository::sqlGenerator()
{
    static const auto sqlGenerator_ = app().getPlugin<SqlGenerator>();
    return sqlGenerator_;
}

inline DbClientPtr UserRepository::dbClient()
{
    static const DbClientPtr dbClient_ = app().getDbClient();
    return dbClient_;
}

inline CoroMapper<SysUser> UserRepository::userMapper()
{
    return CoroMapper<SysUser>{dbClient()};
}
