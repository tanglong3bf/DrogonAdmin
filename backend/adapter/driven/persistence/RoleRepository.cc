#include "domain/authorization/RoleRepository.h"

#include <drogon/HttpAppFramework.h>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::drogon_admin_db;
using namespace tl::sql;

Task<size_t> RoleRepository::countBelongDept(const int32_t deptId) const
{
    auto sql =
        sqlGenerator()->getSql("count_belong_dept", {{"dept_id", deptId}});
    auto result = co_await dbClient()->execSqlCoro(sql);
    co_return result[0][0].as<size_t>();
}

Task<vector<RoleDept>> RoleRepository::getExcludingDeptByDeptId(
    const std::int32_t deptId) const
{
    auto sql = sqlGenerator()->getSql("get_excluding_dept_by_dept_id",
                                      {{"dept_id", deptId}});
    auto result = co_await dbClient()->execSqlCoro(sql);
    std::vector<SysRoleDept> sysRoleDeptList;
    for (const auto &row : result)
    {
        sysRoleDeptList.emplace_back(row);
    }
    co_return buildRoleDeptList(sysRoleDeptList);
}

Task<> RoleRepository::saveRoleDepts(const vector<RoleDept> &roleDeptList) const
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
        Json::Value dataList;
        for (const auto &item : toDelete)
        {
            dataList.append(item.toJson());
        }
        auto sql = sqlGenerator()->getSql("multi_delete_for_role_dept",
                                          {{"data_list", dataList}});
        co_await dbClient()->execSqlCoro(sql);
    }
}

drogon::Task<std::size_t> RoleRepository::countByName(
    const std::string &name) const
{
    Criteria criteria{SysRoleDept::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysRole::Cols::_name, name};
    co_return co_await roleMapper().count(criteria);
}

drogon::Task<std::size_t> RoleRepository::countByCode(
    const std::string &name) const
{
    Criteria criteria{SysRoleDept::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysRole::Cols::_code, name};
    co_return co_await roleMapper().count(criteria);
}

drogon::Task<> RoleRepository::save(const Role &role) const
{
    const auto sysRole = static_cast<SysRole>(role);

    const auto trans = co_await dbClient()->newTransactionCoro();
    // 插入本体
    const auto sysRoleInDb = co_await roleMapper(trans).insert(sysRole);
    // 获取id
    const auto roleId = sysRoleInDb.getValueOfRoleId();

    // 处理关联数据
    auto roleDepts = role.getDepts();
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

    auto sql =
        sqlGenerator()->getSql("multi_insert_role_dept", {{"data", data}});
    co_await trans->execSqlCoro(sql);
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

inline DbClientPtr RoleRepository::dbClient()
{
    static const DbClientPtr dbClient_ = app().getDbClient();
    return dbClient_;
}

inline CoroMapper<SysRole> RoleRepository::roleMapper(
    const std::shared_ptr<drogon::orm::Transaction> &trans)
{
    return CoroMapper<SysRole>{trans == nullptr ? dbClient() : trans};
}

inline CoroMapper<SysRoleDept> RoleRepository::roleDeptMapper()
{
    return CoroMapper<SysRoleDept>{dbClient()};
}
