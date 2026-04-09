#include "domain/organization/user/UserRepository.h"

#include <drogon/HttpAppFramework.h>
#include "common/framework/domain/ChangeableEntity.h"
#include "domain/models/SysUser.h"
#include "domain/models/SysUserRole.h"

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

Task<size_t> UserRepository::countByRole(const int32_t roleId) const
{
    Criteria criteria{SysUserRole::Cols::_role_id, roleId};

    co_return co_await userRoleMapper().count(criteria);
}

drogon::Task<std::size_t> UserRepository::countByUsername(
    const std::string &username) const
{
    Criteria criteria{SysUser::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysUser::Cols::_username, username};

    co_return co_await userMapper().count(criteria);
}

drogon::Task<std::size_t> UserRepository::countByNickname(
    const std::string &nickname) const
{
    Criteria criteria{SysUser::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysUser::Cols::_nickname, nickname};

    co_return co_await userMapper().count(criteria);
}

drogon::Task<> UserRepository::save(User &user) const
{
    switch (user.getChangingStatus())
    {
        case ChangingStatus::NEW:
        {
            const auto trans = co_await dbClient()->newTransactionCoro();
            const auto model = static_cast<SysUser>(user);
            const auto userInDb = co_await userMapper(trans).insert(model);
            if (user.getUserRoles().size() > 0)
            {
                Json::Value data;
                for (auto &userRole : user.getUserRoles())
                {
                    const_cast<UserRole &>(userRole).setUserId(
                        userInDb.getValueOfUserId());
                    const auto item = static_cast<SysUserRole>(userRole);
                    data.append(item.toJson());
                }
                const auto sql =
                    sqlGenerator()->getSql("multi_insert_user_role",
                                           {{"data", data}});
                co_await trans->execSqlCoro(sql);
            }
            co_return;
        }
        case ChangingStatus::UPDATED:
            co_return;
        case ChangingStatus::DELETED:
            co_return;
        case ChangingStatus::UNCHANGED:
            LOG_WARN << "无需修改的数据调用了save，请检查代码逻辑";
            co_return;
    }
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

inline CoroMapper<SysUser> UserRepository::userMapper(
    const shared_ptr<Transaction> trans)
{
    return CoroMapper<SysUser>{trans == nullptr ? dbClient() : trans};
}

inline CoroMapper<SysUserRole> UserRepository::userRoleMapper()
{
    return CoroMapper<SysUserRole>{dbClient()};
}
