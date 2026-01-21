#pragma once

#include "common/framework/domain/AuditableEntity.h"
#include "common/framework/domain/ChangeableEntity.h"
#include "domain/models/SysRoleDept.h"
#include "RoleDeptRelationType.hpp"

/**
 * @brief 角色部门关联实体
 */
class RoleDept : public AuditableEntity, public ChangeableEntity
{
    using SysRoleDept = drogon_model::drogon_admin_db::SysRoleDept;

    std::optional<std::int32_t> id_;     ///< 主键 id
    std::int32_t roleId_;                ///< 角色id
    std::int32_t deptId_;                ///< 部门id
    RoleDeptRelationType relationType_;  ///< 关联关系

  public:
    // 和model类互转
    explicit RoleDept(const SysRoleDept &sysRoleDept);
    explicit operator SysRoleDept() const;
};
