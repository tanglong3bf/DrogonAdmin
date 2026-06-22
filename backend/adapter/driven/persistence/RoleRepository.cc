#include "domain/authorization/RoleRepository.h"

#include "domain/models/SysRoleDept.h"
#include "common/framework/domain/ChangeableEntity.h"
#include <drogon/HttpAppFramework.h>
#include <drogon/orm/Criteria.h>
#include <trantor/utils/Date.h>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::drogon_admin_db;
using namespace tl::sql;

Task<size_t> RoleRepository::countBelongDept(const std::int32_t deptId,
                                             const DbClientPtr &dbClient) const
{
    auto sql =
        sqlGenerator()->getSql("count_belong_dept", {{"dept_id", deptId}});
    auto result = co_await dbClient->execSqlCoro(sql);
    co_return result[0][0].as<size_t>();
}

Task<vector<RoleDept>> RoleRepository::getExcludingDeptByDeptId(
    const std::int32_t deptId,
    const DbClientPtr &dbClient) const
{
    auto sql = sqlGenerator()->getSql("get_excluding_dept_by_dept_id",
                                      {{"dept_id", deptId}});
    auto result = co_await dbClient->execSqlCoro(sql);
    vector<SysRoleDept> sysRoleDeptList;
    for (const auto &row : result)
    {
        sysRoleDeptList.emplace_back(row);
    }
    co_return buildRoleDeptList(sysRoleDeptList);
}

Task<> RoleRepository::saveRoleDepts(const vector<RoleDept> &roleDeptList,
                                     const DbClientPtr &dbClient) const
{
    vector<SysRoleDept> newData;
    vector<SysRoleDept> toUpdate;
    vector<SysRoleDept> toDelete;

    for (const auto &roleDept : roleDeptList)
    {
        const auto status = roleDept.getChangingStatus();
        const auto item = static_cast<SysRoleDept>(roleDept);
        switch (status)
        {
            case ChangingStatus::NEW:
                newData.emplace_back(item);
                break;
            case ChangingStatus::UPDATED:
                toUpdate.emplace_back(item);
                break;
            case ChangingStatus::DELETED:
                toDelete.emplace_back(item);
                break;
            case ChangingStatus::UNCHANGED:
                // 未做更改，无需操作数据库
                break;
        }
    }

    if (toDelete.size() > 0)
    {
        const auto deptIdsView =
            toDelete | views::transform([](const auto &data) {
                return data.getValueOfDeptId();
            });
        vector<std::int32_t> deptIds(deptIdsView.begin(), deptIdsView.end());
        auto criteria =
            Criteria{SysRoleDept::Cols::_role_id,
                     toDelete[0].getValueOfRoleId()} &&
            Criteria{SysRoleDept::Cols::_dept_id, CompareOperator::In, deptIds};
        co_await roleDeptMapper(dbClient).deleteBy(criteria);
    }
}

Task<size_t> RoleRepository::countByName(const string &name) const
{
    Criteria criteria{SysRole::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysRole::Cols::_name, name};
    co_return co_await roleMapper().count(criteria);
}

Task<size_t> RoleRepository::countByCode(const string &code) const
{
    Criteria criteria{SysRole::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysRole::Cols::_code, code};
    co_return co_await roleMapper().count(criteria);
}

Task<size_t> RoleRepository::countByIds(const vector<int32_t> &roleIds) const
{
    if (roleIds.size() == 0)
    {
        co_return 0;
    }
    Criteria criteria{SysRole::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria &&
               Criteria{SysRole::Cols::_role_id, CompareOperator::In, roleIds};
    co_return co_await roleMapper().count(criteria);
}

Task<> RoleRepository::save(Role &role) const
{
    // 由于这里涉及到多表操作，一定需要开启事务
    // 如果由外界传入，则无法保证
    const auto trans = co_await app().getDbClient()->newTransactionCoro();
    switch (role.getChangingStatus())
    {
        case ChangingStatus::NEW:
        {
            const auto sysRole = static_cast<SysRole>(role);
            // 插入本体
            const auto sysRoleInDb = co_await roleMapper(trans).insert(sysRole);
            // 获取id
            const auto roleId = sysRoleInDb.getValueOfRoleId();

            // 处理关联数据
            auto roleDepts = role.getRoleDepts();
            if (roleDepts.size() == 0)
            {
                co_return;
            }
            for (auto &dept : roleDepts)
            {
                dept.setRoleId(roleId);
            }

            // 关联数据转json
            Json::Value data{Json::arrayValue};
            for (const auto &dept : roleDepts)
            {
                const auto sysRoleDept = static_cast<SysRoleDept>(dept);
                data.append(sysRoleDept.toJson());
            }

            auto sql = sqlGenerator()->getSql("multi_insert_role_dept",
                                              {{"data", data}});
            co_await trans->execSqlCoro(sql);
            co_return;
        }
        case ChangingStatus::DELETED:
        {
            // 删除本体
            co_await roleMapper(trans).updateBy({SysRole::Cols::_deleted_by,
                                                 SysRole::Cols::_deleted_time},
                                                {SysRole::Cols::_role_id,
                                                 *role.getRoleId()},
                                                *role.getDeletedBy(),
                                                *role.getDeletedTime());
            // 处理关联数据
            auto roleDepts = role.getRoleDepts();
            if (roleDepts.size() == 0)
            {
                co_return;
            }
            // 关联数据转json
            Json::Value data{Json::arrayValue};
            for (const auto &dept : roleDepts)
            {
                Json::Value json{Json::objectValue};
                json["role_id"] = *dept.getRoleId();
                json["dept_id"] = dept.getDeptId();
                data.append(json);
            }
            auto sql = sqlGenerator()->getSql("multi_delete_role_dept",
                                              {{"data", data}});
            co_await trans->execSqlCoro(sql);
            co_return;
        }
        case ChangingStatus::UPDATED:
        {
            SysRole sysRole = static_cast<SysRole>(role);
            co_await roleMapper(trans).update(sysRole);

            // 处理关联数据
            auto roleDepts = role.getRoleDepts();
            if (roleDepts.size() == 0)
            {
                co_return;
            }

            // 关联数据转json
            Json::Value toDelete{Json::arrayValue};
            Json::Value newDepts{Json::arrayValue};
            for (const auto &dept : roleDepts)
            {
                Json::Value json{Json::objectValue};
                json["role_id"] = *dept.getRoleId();
                json["dept_id"] = dept.getDeptId();
                if (dept.getChangingStatus() == ChangingStatus::NEW)
                {
                    json["created_by"] = *dept.getCreatedBy();
                    json["created_time"] =
                        dept.getCreatedTime()->toDbStringLocal();
                    newDepts.append(json);
                }
                else if (dept.getChangingStatus() == ChangingStatus::DELETED)
                {
                    toDelete.append(json);
                }
            }

            // 获取sql并执行
            if (toDelete.size() > 0)
            {
                auto deleteSql =
                    sqlGenerator()->getSql("multi_delete_role_dept",
                                           {{"data", toDelete}});
                co_await trans->execSqlCoro(deleteSql);
            }
            if (newDepts.size() > 0)
            {
                auto insertSql =
                    sqlGenerator()->getSql("multi_insert_role_dept",
                                           {{"data", newDepts}});
                co_await trans->execSqlCoro(insertSql);
            }
            co_return;
        }
        case ChangingStatus::UNCHANGED:
            co_return;
    }
}

Task<Role> RoleRepository::getById(const std::int32_t roleId) const
{
    const auto sysRole = co_await roleMapper().findByPrimaryKey(roleId);
    Role role{sysRole};

    Criteria criteria{SysRoleDept::Cols::_role_id, roleId};

    const auto sysRoleDepts = co_await roleDeptMapper().findBy(criteria);

    auto roleDepts = buildRoleDeptList(sysRoleDepts);

    role.setRoleDepts(roleDepts);
    co_return role;
}

drogon::Task<std::vector<Role>> RoleRepository::getByIds(
    const std::vector<std::int32_t> &roleIds,
    const bool withRelation) const
{
    if (roleIds.empty())
    {
        co_return std::vector<Role>{};
    }

    Criteria criteria{SysRole::Cols::_role_id, CompareOperator::In, roleIds};
    const auto sysRoles = co_await roleMapper().findBy(criteria);
    auto roles = buildRoleList(sysRoles);

    if (!withRelation)
    {
        co_return roles;
    }

    const auto sysRoleDepts = co_await roleDeptMapper().findBy(criteria);

    std::unordered_map<std::int32_t, std::vector<SysRoleDept>> deptMap;
    for (const auto &dept : sysRoleDepts)
    {
        deptMap[dept.getValueOfRoleId()].push_back(dept);
    }

    for (auto &role : roles)
    {
        auto roleId = role.getRoleId();
        if (auto it = deptMap.find(*roleId); it != deptMap.end())
        {
            for (const auto &dept : it->second)
            {
                role.addRoleDept(RoleDept{dept});
            }
        }
    }

    co_return roles;
}

vector<Role> RoleRepository::buildRoleList(
    const vector<SysRole> &sysRoleList) const
{
    vector<Role> result;
    for (const auto &sysRole : sysRoleList)
    {
        result.emplace_back(static_cast<Role>(sysRole));
    }
    return result;
}

vector<RoleDept> RoleRepository::buildRoleDeptList(
    const vector<SysRoleDept> &sysRoleDeptList) const
{
    vector<RoleDept> result;
    for (const auto &sysRoleDept : sysRoleDeptList)
    {
        result.emplace_back(static_cast<RoleDept>(sysRoleDept));
    }
    return result;
}

inline SqlGenerator *RoleRepository::sqlGenerator()
{
    static const auto sqlGenerator_ = app().getPlugin<SqlGenerator>();
    return sqlGenerator_;
}

inline CoroMapper<SysRole> RoleRepository::roleMapper(
    const DbClientPtr &dbClient)
{
    return CoroMapper<SysRole>{dbClient};
}

inline CoroMapper<SysRoleDept> RoleRepository::roleDeptMapper(
    const DbClientPtr &dbClient)
{
    return CoroMapper<SysRoleDept>{dbClient};
}
