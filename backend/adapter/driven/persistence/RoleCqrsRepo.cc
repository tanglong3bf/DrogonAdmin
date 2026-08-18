#include "application/iam/role/RoleCqrsRepo.h"

#include "common/util/AttrUtils.hpp"
#include "common/util/rangesUtils.hpp"
#include <drogon/HttpAppFramework.h>
#include <drogon/orm/Criteria.h>
#include <ranges>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::drogon_admin_db;
using namespace tl::sql;
using namespace drogon_admin;

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
    const size_t maxPage,
    const AttributesPtr &attr) const
{
    ParamList params;
    const std::int32_t page =
        maxPage < request.page() ? maxPage : request.page();
    if (page != request.page())
    {
        addWarn(attr, "查询页码超出范围，已自动调整到最后一页");
    }
    params["offset"] =
        static_cast<std::int32_t>(request.pageSize() * (page - 1));
    params["limit"] = static_cast<std::int32_t>(request.pageSize());
    if (request.deptId())
    {
        params["dept_id"] = *request.deptId();
    }
    if (request.name())
    {
        params["name"] = *request.name();
    }
    const auto sql = sqlGenerator()->getSql("get_role_list", params);
    const auto dbResult = co_await dbClient()->execSqlCoro(sql);
    auto roleList = buildList(dbResult);

    vector<std::int32_t> roleIds{};
    for (const auto &role : roleList)
    {
        roleIds.push_back(*role.roleId());
    }

    auto roleDeptList = co_await roleDeptMapper().findBy(
        Criteria{SysRoleDept::Cols::_role_id, CompareOperator::In, roleIds});

    for (auto &role : roleList)
    {
        role.restoreDepts(roleDeptList |
                          views::filter([&role](const SysRoleDept &rd) {
                              return rd.getValueOfRoleId() == role.roleId();
                          }) |
                          ranges_utils::to<vector<SysRoleDept>>());
    }

    co_return buildRoleResponseList(roleList);
}

Task<vector<AssignableRoleResponse>> RoleCqrsRepo::getAssignableRoles(
    const std::int32_t deptId) const
{
    const auto sql = sqlGenerator()->getSql("get_assignable_roles");
    const auto dbResult = co_await dbClient()->execSqlCoro(sql, deptId);
    co_return buildAssignableList(dbResult);
}

vector<Role> RoleCqrsRepo::buildList(const Result &dbResult) const
{
    vector<Role> result;
    for (const auto &row : dbResult)
    {
        Role role{SysRole{row}};
        result.push_back(role);
    }
    return result;
}

std::vector<RoleResponse> RoleCqrsRepo::buildRoleResponseList(
    const std::vector<Role> &roleList) const
{
    return roleList | views::transform([this](const auto &role) {
               RoleResponse roleResponse{role};
               for (const auto dept : role.roleDepts())
               {
                   roleResponse.addRoleDept(
                       static_cast<RoleDeptResponse>(dept));
               }
               return roleResponse;
           }) |
           ranges_utils::to<vector>();
}

vector<AssignableRoleResponse> RoleCqrsRepo::buildAssignableList(
    const Result &dbResult) const
{
    vector<AssignableRoleResponse> result;
    for (const auto &row : dbResult)
    {
        AssignableRoleResponse assignableRoleResponse{
            row["role_id"].as<std::int32_t>(),
            row["name"].as<string>(),
            row["version"].as<std::int32_t>()};
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
