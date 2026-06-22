#include "domain/organization/user/UserRepository.h"

#include "common/framework/domain/ChangeableEntity.h"
#include "domain/models/SysUser.h"
#include "domain/models/SysUserRole.h"
#include <drogon/HttpAppFramework.h>
#include <unordered_map>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::drogon_admin_db;
using namespace tl::sql;

Task<size_t> UserRepository::countByDept(const std::int32_t deptId) const
{
    Criteria criteria{SysUser::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysUser::Cols::_dept_id, deptId};

    co_return co_await userMapper().count(criteria);
}

Task<size_t> UserRepository::countByRole(const std::int32_t roleId) const
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
    switch (user.changingStatus())
    {
        case ChangingStatus::NEW:
        {
            const auto trans = co_await dbClient()->newTransactionCoro();
            auto model = static_cast<SysUser>(user);
            // 补充默认密码
            model.setPassword("123456");
            const auto userInDb = co_await userMapper(trans).insert(model);
            if (user.userRoles.size() > 0)
            {
                Json::Value data;
                for (auto &userRole : user.userRoles)
                {
                    const_cast<UserRole &>(userRole).userId =
                        userInDb.getValueOfUserId();
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
        {
            const auto trans = co_await dbClient()->newTransactionCoro();
            const auto model = static_cast<SysUser>(user);
            co_await userMapper(trans).update(model);

            Json::Value toInsert{Json::arrayValue};
            Json::Value toDelete{Json::arrayValue};

            for (auto &userRole : user.userRoles)
            {
                const auto status = userRole.changingStatus();
                const auto item = static_cast<SysUserRole>(userRole);
                switch (status)
                {
                    case ChangingStatus::NEW:
                        toInsert.append(item.toJson());
                        break;
                    case ChangingStatus::DELETED:
                        toDelete.append(item.toJson());
                        break;
                    case ChangingStatus::UNCHANGED:
                        // 未做更改，无需操作数据库
                        break;
                    case ChangingStatus::UPDATED:
                        throw std::runtime_error("不支持更新用户角色");
                        break;
                }
            }
            if (toInsert.size() > 0)
            {
                const auto insertSql =
                    sqlGenerator()->getSql("multi_insert_user_role",
                                           {{"data", toInsert}});
                co_await trans->execSqlCoro(insertSql);
            }
            if (toDelete.size() > 0)
            {
                const auto deleteSql =
                    sqlGenerator()->getSql("multi_delete_user_role",
                                           {{"data", toDelete}});
                co_await trans->execSqlCoro(deleteSql);
            }
            co_return;
        }
        case ChangingStatus::DELETED:
        {
            const auto trans = co_await dbClient()->newTransactionCoro();
            const auto model = static_cast<SysUser>(user);
            // 本体删除(软删除)
            co_await userMapper(trans).update(model);
            // 关联数据删除
            co_await userRoleMapper(trans).deleteBy(
                Criteria{SysUserRole::Cols::_user_id, *user.userId});

            co_return;
        }
        case ChangingStatus::UNCHANGED:
            LOG_WARN << "无需修改的数据调用了save，请检查代码逻辑";
            co_return;
    }
}

Task<User> UserRepository::getById(const std::int32_t userId,
                                   bool withRelation) const
{
    const auto sysUser = co_await userMapper().findOne(
        Criteria{SysUser::Cols::_deleted_by, CompareOperator::IsNull} &&
        Criteria{SysUser::Cols::_user_id, userId});
    User user{sysUser};
    if (withRelation)
    {
        const auto sysUserRoles = co_await userRoleMapper().findBy(
            Criteria{SysUserRole::Cols::_user_id, userId});
        const auto userRoles = buildUserRoleList(sysUserRoles);
        user.setUserRoles(userRoles);
    }
    co_return user;
}

Task<unordered_map<int32_t, size_t>> UserRepository::countByDeptAndRoles(
    const std::int32_t deptId,
    const vector<std::int32_t> &roleIds) const
{
    LOG_TRACE << "统计部门下角色的用户数量，deptId=" << deptId
              << ", roleIds=" << roleIds.size() << "个角色";
    ParamList param;
    param["dept_id"] = deptId;
    Json::Value roleIdsJson(Json::arrayValue);
    for (const auto &id : roleIds)
    {
        roleIdsJson.append(id);
    }
    param["role_ids"] = roleIdsJson;
    const auto sql =
        sqlGenerator()->getSql("count_user_by_dept_and_roles", param);
    // used_count, ur.role_id
    const auto dbResult = co_await dbClient()->execSqlCoro(sql);
    unordered_map<int32_t, size_t> result;
    for (const auto &row : dbResult)
    {
        result[row["role_id"].as<int32_t>()] = row["used_count"].as<size_t>();
    }
    co_return result;
}

drogon::Task<std::map<std::int32_t, std::size_t>> UserRepository::
    countByRoleList(const std::vector<std::int32_t> &roleIds) const
{
    LOG_TRACE << "统计角色的用户数量，roleIds=" << roleIds.size() << "个角色";
    ParamList param;
    Json::Value roleIdsJson(Json::arrayValue);
    for (const auto &id : roleIds)
    {
        roleIdsJson.append(id);
    }
    param["role_ids"] = roleIdsJson;
    const auto sql = sqlGenerator()->getSql("count_user_by_role_list", param);
    // used_count, ur.role_id
    const auto dbResult = co_await dbClient()->execSqlCoro(sql);
    std::map<int32_t, size_t> result;
    for (const auto &row : dbResult)
    {
        result[row["role_id"].as<int32_t>()] = row["used_count"].as<size_t>();
    }
    co_return result;
}

Task<unordered_map<std::int32_t, size_t>> UserRepository::
    countUsersWithRolePerDepartment(const std::int32_t roleId) const
{
    const auto sql =
        sqlGenerator()->getSql("count_users_with_role_per_department",
                               {{"role_id", roleId}});
    const auto dbResult = co_await dbClient()->execSqlCoro(sql);
    unordered_map<int32_t, size_t> result;
    for (const auto &row : dbResult)
    {
        result[row["dept_id"].as<int32_t>()] = row["user_count"].as<size_t>();
    }
    co_return result;
}

Task<vector<std::int32_t>> UserRepository::getDeptIdsByRoleId(
    const std::int32_t roleId) const
{
    const auto sql = sqlGenerator()->getSql("get_dept_ids_by_role_id",
                                            {{"role_id", roleId}});
    const auto dbResult = co_await dbClient()->execSqlCoro(sql);
    vector<int32_t> deptIds;
    for (const auto &row : dbResult)
    {
        deptIds.push_back(row["dept_id"].as<int32_t>());
    }
    co_return deptIds;
}

Task<size_t> UserRepository::countByRoleNotInDepts(
    const std::int32_t roleId,
    const vector<std::int32_t> &deptIds) const
{
    ParamList param;
    param["role_id"] = roleId;
    Json::Value deptIdsJson(Json::arrayValue);
    for (const auto &id : deptIds)
    {
        deptIdsJson.append(id);
    }
    param["dept_ids"] = deptIdsJson;
    const auto sql =
        sqlGenerator()->getSql("count_user_by_role_not_in_depts", param);
    const auto dbResult = co_await dbClient()->execSqlCoro(sql);
    co_return dbResult[0][0].as<size_t>();
}

Task<size_t> UserRepository::countByRoleInDepts(
    const std::int32_t roleId,
    const vector<std::int32_t> &deptIds) const
{
    ParamList param;
    param["role_id"] = roleId;
    Json::Value deptIdsJson(Json::arrayValue);
    for (const auto &id : deptIds)
    {
        deptIdsJson.append(id);
    }
    param["dept_ids"] = deptIdsJson;
    const auto sql =
        sqlGenerator()->getSql("count_user_by_role_in_depts", param);
    const auto dbResult = co_await dbClient()->execSqlCoro(sql);
    co_return dbResult[0][0].as<size_t>();
}

Task<map<int32_t, size_t>> UserRepository::countUsersPerRoleInDepartment(
    const int32_t deptId,
    const vector<int32_t> &roleIds) const
{
    ParamList param;
    param["dept_id"] = deptId;
    Json::Value roleIdsJson(Json::arrayValue);
    for (const auto &id : roleIds)
    {
        roleIdsJson.append(id);
    }
    param["role_ids"] = roleIdsJson;
    const auto sql =
        sqlGenerator()->getSql("count_users_per_role_in_department", param);
    const auto dbResult = co_await dbClient()->execSqlCoro(sql);
    map<int32_t, size_t> result;
    for (const auto &row : dbResult)
    {
        result[row["role_id"].as<int32_t>()] = row["user_count"].as<size_t>();
    }
    co_return result;
}

vector<UserRole> UserRepository::buildUserRoleList(
    const vector<SysUserRole> &sysUserRoles) const
{
    vector<UserRole> userRoles;
    for (const auto &sysUserRole : sysUserRoles)
    {
        userRoles.push_back(UserRole{sysUserRole});
    }
    return userRoles;
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

inline CoroMapper<SysUserRole> UserRepository::userRoleMapper(
    const shared_ptr<Transaction> trans)
{
    return CoroMapper<SysUserRole>{trans == nullptr ? dbClient() : trans};
}
