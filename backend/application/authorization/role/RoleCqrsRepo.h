#pragma once

#include "dto/RoleQueryRequest.h"
#include "dto/AssignableRoleResponse.h"
#include "dto/RoleResponse.h"
#include "domain/models/SysRole.h"
#include "common/framework/DrAdminObject.hpp"
#include "SqlGenerator/src/SqlGenerator.h"
#include <drogon/orm/DbClient.h>
#include <drogon/utils/coroutine.h>

/**
 * 角色查询仓库
 */
class RoleCqrsRepo : public DrAdminObject<RoleCqrsRepo>
{
    using SysRole = drogon_model::drogon_admin_db::SysRole;
    using RoleMapper = drogon::orm::CoroMapper<SysRole>;
    using SysRoleDept = drogon_model::drogon_admin_db::SysRoleDept;
    using RoleDeptMapper = drogon::orm::CoroMapper<SysRoleDept>;

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
        const RoleQueryRequest &request,
        const std::size_t maxPage /*很不优雅*/,
        const drogon::AttributesPtr &attr) const;

    /**
     * @brief 获取指定部门可分配的角色
     */
    drogon::Task<std::vector<AssignableRoleResponse>> getAssignableRoles(
        const std::int32_t deptId) const;

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
