#pragma once

#include <memory>
#include <drogon/orm/CoroMapper.h>
#include <drogon/utils/coroutine.h>
#include "SqlGenerator/src/SqlGenerator.h"
#include "common/framework/DrAdminObject.hpp"
#include "Role.h"
#include "domain/models/SysRoleDept.h"
#include "RoleDept.h"

/**
 * @brief 角色仓库
 */
class RoleRepository : public DrAdminObject<RoleRepository>
{
    using DbClientPtr = drogon::orm::DbClientPtr;
    using SqlGenerator = tl::sql::SqlGenerator;
    using RoleMapper =
        drogon::orm::CoroMapper<drogon_model::drogon_admin_db::SysRole>;
    using SysRoleDept = drogon_model::drogon_admin_db::SysRoleDept;
    using RoleDeptMapper = drogon::orm::CoroMapper<SysRoleDept>;

  public:
    /**
     * @brief 统计部门被正向关联的次数
     */
    drogon::Task<std::size_t> countBelongDept(const std::int32_t deptId) const;

    /**
     * @brief 获取部门被哪些角色排除
     */
    drogon::Task<std::vector<RoleDept>> getExcludingDeptByDeptId(
        const std::int32_t deptId) const;

    /**
     * @brief 存储关联数据
     */
    drogon::Task<> saveRoleDepts(
        const std::vector<RoleDept> &roleDeptList) const;

    /**
     * @brief 按照名称统计角色数量
     */
    drogon::Task<std::size_t> countByName(const std::string &name) const;

    /**
     * @brief 按照角色代码统计角色数量
     */
    drogon::Task<std::size_t> countByCode(const std::string &code) const;

    /**
     * @brief 存储角色
     */
    drogon::Task<> save(Role &role) const;

    /**
     * @brief 根据id获取角色
     */
    drogon::Task<Role> getById(const std::int32_t roleId) const;

  protected:
    std::vector<RoleDept> buildRoleDeptList(
        const std::vector<SysRoleDept> &sysRoleDeptList) const;

  private:
    static SqlGenerator *sqlGenerator();
    static DbClientPtr dbClient();
    static RoleMapper roleMapper(
        const std::shared_ptr<drogon::orm::Transaction> &trans = nullptr);
    static RoleDeptMapper roleDeptMapper();
};

using RoleRepositoryPtr = std::shared_ptr<RoleRepository>;
