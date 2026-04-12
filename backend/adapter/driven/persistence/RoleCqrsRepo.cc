#include "application/authorization/role/RoleCqrsRepo.h"

#include "domain/authorization/RoleDept.h"
#include "domain/authorization/Role.h"
#include "domain/models/SysRole.h"
#include <drogon/HttpAppFramework.h>
#include <drogon/orm/Criteria.h>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::drogon_admin_db;
using namespace tl::sql;

Task<size_t> RoleCqrsRepo::countByNameAndDeptId(
    const optional<string> &name,
    const optional<std::int32_t> &deptId) const
{
    ParamList params;
    if (deptId)
    {
        params["dept_id"] = *deptId;
    }
    if (name)
    {
        params["name"] = *name;
    }
    const auto sql =
        sqlGenerator()->getSql("count_role_by_name_and_dept_id", params);
    const auto dbResult = co_await dbClient()->execSqlCoro(sql);
    co_return dbResult[0][0].as<size_t>();
}

Task<vector<RoleResponse>> RoleCqrsRepo::getRoleList(
    const RoleQueryRequest &request,
    const size_t maxPage) const
{
    ParamList params;
    const std::int32_t page =
        maxPage < request.getPage() ? maxPage : request.getPage();
    params["offset"] =
        static_cast<std::int32_t>(request.getPageSize() * (page - 1));
    params["limit"] = static_cast<std::int32_t>(request.getPageSize());
    if (request.getDeptId())
    {
        params["dept_id"] = *request.getDeptId();
    }
    if (request.getName())
    {
        params["name"] = *request.getName();
    }
    const auto sql = sqlGenerator()->getSql("get_role_list", params);
    const auto dbResult = co_await dbClient()->execSqlCoro(sql);
    auto roleList = buildList(dbResult);

    vector<std::int32_t> roleIds{};
    for (const auto &role : roleList)
    {
        roleIds.push_back(role.getRoleId());
    }

    auto roleDeptList = co_await roleDeptMapper().findBy(
        Criteria{SysRoleDept::Cols::_role_id, CompareOperator::In, roleIds});

    for (const auto &roleDept : roleDeptList)
    {
        for (auto &role : roleList)
        {
            if (role.getRoleId() == roleDept.getValueOfRoleId())
            {
                role.addRoleDept(RoleDept{roleDept});
            }
        }
    }

    co_return roleList;
}

Task<vector<AssignableRoleResponse>> RoleCqrsRepo::getAssignableRoles(
    const std::int32_t deptId) const
{
    const auto sql = sqlGenerator()->getSql("get_assignable_roles");
    const auto dbResult = co_await dbClient()->execSqlCoro(sql, deptId);
    co_return buildAssignableList(dbResult);
}

vector<RoleResponse> RoleCqrsRepo::buildList(const Result &dbResult) const
{
    vector<RoleResponse> result;
    for (const auto &row : dbResult)
    {
        RoleResponse roleResponse{Role{SysRole{row}}};
        result.push_back(roleResponse);
    }
    return result;
}

vector<AssignableRoleResponse> RoleCqrsRepo::buildAssignableList(
    const Result &dbResult) const
{
    vector<AssignableRoleResponse> result;
    for (const auto &row : dbResult)
    {
        AssignableRoleResponse assignableRoleResponse{
            row["role_id"].as<std::int32_t>(), row["name"].as<string>()};
        result.push_back(assignableRoleResponse);
    }
    return result;
}

SqlGenerator *RoleCqrsRepo::sqlGenerator()
{
    static auto *plugin = app().getPlugin<SqlGenerator>();
    return plugin;
}

DbClientPtr RoleCqrsRepo::dbClient()
{
    static auto dbClient = app().getDbClient();
    return dbClient;
}

inline CoroMapper<SysRole> RoleCqrsRepo::roleMapper()
{
    return CoroMapper<SysRole>{dbClient()};
}

inline CoroMapper<SysRoleDept> RoleCqrsRepo::roleDeptMapper()
{
    return CoroMapper<SysRoleDept>{dbClient()};
}
