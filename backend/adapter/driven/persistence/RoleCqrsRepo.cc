#include "application/authorization/RoleCqrsRepo.h"

#include <drogon/HttpAppFramework.h>
#include <drogon/orm/Criteria.h>
#include "domain/models/SysRole.h"
#include "domain/authorization/Role.h"

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::drogon_admin_db;
using namespace tl::sql;

Task<size_t> RoleCqrsRepo::countByNameAndDeptId(
    const optional<string> &name,
    const optional<int32_t> &deptId) const
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
    const GetRoleListRequest &request,
    const int32_t maxPage) const
{
    ParamList params;
    const int32_t page =
        maxPage < request.getPage() ? maxPage : request.getPage();
    params["offset"] = request.getPageSize() * (page - 1);
    params["limit"] = request.getPageSize();
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
    co_return buildList(dbResult);
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

inline CoroMapper<SysRole> RoleCqrsRepo::roleMapper() const
{
    return CoroMapper<SysRole>{dbClient()};
}
