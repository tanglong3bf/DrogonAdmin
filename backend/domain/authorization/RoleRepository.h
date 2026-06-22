#pragma once

#include "Role.h"
#include "RoleDept.h"
#include "domain/models/SysRoleDept.h"
#include "common/framework/DrAdminObject.hpp"
#include "SqlGenerator/src/SqlGenerator.h"
#include <drogon/HttpAppFramework.h>
#include <drogon/orm/CoroMapper.h>
#include <drogon/utils/coroutine.h>
#include <memory>
#include <vector>

/**
 * @brief 角色仓库
 */
class RoleRepository : public DrAdminObject<RoleRepository>
{
    using DbClientPtr = drogon::orm::DbClientPtr;
    using SqlGenerator = tl::sql::SqlGenerator;
    using SysRole = drogon_model::drogon_admin_db::SysRole;
    using RoleMapper = drogon::orm::CoroMapper<SysRole>;
    using SysRoleDept = drogon_model::drogon_admin_db::SysRoleDept;
    using RoleDeptMapper = drogon::orm::CoroMapper<SysRoleDept>;

  public:
    /**
     * @brief 统计部门被正向关联的次数
     */
    drogon::Task<std::size_t> countBelongDept(
        const std::int32_t deptId,
        const DbClientPtr & = drogon::app().getDbClient()) const;

    /**
     * @brief 获取部门被哪些角色排除
     */
    drogon::Task<std::vector<RoleDept>> getExcludingDeptByDeptId(
        const std::int32_t deptId,
        const DbClientPtr & = drogon::app().getDbClient()) const;

    /**
     * @brief 存储关联数据
     */
    drogon::Task<> saveRoleDepts(
        const std::vector<RoleDept> &roleDeptList,
        const DbClientPtr & = drogon::app().getDbClient()) const;

    /**
     * @brief 按照名称统计角色数量
     */
    drogon::Task<std::size_t> countByName(const std::string &name) const;

    /**
     * @brief 按照角色代码统计角色数量
     */
    drogon::Task<std::size_t> countByCode(const std::string &code) const;

    /**
     * @brief 按照id列表统计角色数量
     */
    drogon::Task<std::size_t> countByIds(
        const std::vector<std::int32_t> &roleIds) const;
    /**
     * @brief 存储角色
     */
    drogon::Task<> save(Role &role) const;

    /**
     * @brief 根据id获取角色
     */
    drogon::Task<Role> getById(const std::int32_t roleId) const;

    /**
     * @brief 根据id列表获取角色
     */
    drogon::Task<std::vector<Role>> getByIds(
        const std::vector<std::int32_t> &roleIds,
        const bool withRelation) const;

  protected:
    std::vector<Role> buildRoleList(
        const std::vector<SysRole> &sysRoleList) const;
    std::vector<RoleDept> buildRoleDeptList(
        const std::vector<SysRoleDept> &sysRoleDeptList) const;

  private:
    static SqlGenerator *sqlGenerator();
    static RoleMapper roleMapper(
        const DbClientPtr &dbClient = drogon::app().getDbClient());
    static RoleDeptMapper roleDeptMapper(
        const DbClientPtr &dbClient = drogon::app().getDbClient());
};

using RoleRepositoryPtr = std::shared_ptr<RoleRepository>;
