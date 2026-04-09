#pragma once

#include <drogon/orm/DbClient.h>
#include <drogon/utils/coroutine.h>
#include "SqlGenerator/src/SqlGenerator.h"
#include "dto/GetRoleListRequest.h"
#include "dto/AssignableRoleResponse.h"
#include "common/framework/DrAdminObject.hpp"
#include "dto/RoleResponse.h"
#include "domain/models/SysRole.h"

class RoleCqrsRepo : public DrAdminObject<RoleCqrsRepo>
{
    using RoleMapper =
        drogon::orm::CoroMapper<drogon_model::drogon_admin_db::SysRole>;
    using RoleDeptMapper =
        drogon::orm::CoroMapper<drogon_model::drogon_admin_db::SysRoleDept>;

  public:
    /**
     * @brief 统计指定部门可用的指定名称的角色数量
     */
    drogon::Task<std::size_t> countByNameAndDeptId(
        const std::optional<std::string> &name,
        const std::optional<std::int32_t> &deptId) const;

    /**
     * @brief 获取角色列表
     *
     * maxPage大于零时，无视request.page
     */
    drogon::Task<std::vector<RoleResponse>> getRoleList(
        const GetRoleListRequest &request,
        const int32_t maxPage /*很不优雅*/) const;

    /**
     * @brief 获取指定部门可分配的角色
     */
    drogon::Task<std::vector<AssignableRoleResponse>> getAssignableRoles(
        const int32_t deptId) const;

  protected:
    std::vector<RoleResponse> buildList(
        const drogon::orm::Result &dbResult) const;

    std::vector<AssignableRoleResponse> buildAssignableList(
        const drogon::orm::Result &dbResult) const;

  private:
    static tl::sql::SqlGenerator *sqlGenerator();
    static drogon::orm::DbClientPtr dbClient();
    static RoleMapper roleMapper();
    static RoleDeptMapper roleDeptMapper();
};

using RoleCqrsRepoPtr = std::shared_ptr<RoleCqrsRepo>;
