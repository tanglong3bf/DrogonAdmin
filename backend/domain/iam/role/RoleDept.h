#pragma once

#include "domain/models/SysRoleDept.h"
#include "common/framework/domain/ChangeableEntity.h"
#include "common/util/Utilities.hpp"

/**
 * @brief 角色部门关联实体
 */
class RoleDept : public ChangeableEntity
{
    using SysRoleDept = drogon_model::drogon_admin_db::SysRoleDept;
    friend class Role;
    friend class RoleRepository;

  public:
    GETTER(roleId)
    GETTER(deptId)
    GETTER(createdBy)
    GETTER(createdTime)

    RoleDept(const std::int32_t deptId);
    RoleDept(const std::int32_t deptId, int32_t createdBy);

    /// @group 和model类互转
    /// @{
    explicit RoleDept(const SysRoleDept &sysRoleDept);
    explicit operator SysRoleDept() const;
    /// @}

    std::optional<std::int32_t> roleId_;        ///< 角色id
    std::int32_t deptId_;                       ///< 部门id
    std::optional<std::int32_t> createdBy_;     ///< 创建者id
    std::optional<trantor::Date> createdTime_;  ///< 创建时间
};
