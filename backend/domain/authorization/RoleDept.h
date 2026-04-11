#pragma once

#include "common/util/Utilities.hpp"
#include "common/framework/domain/ChangeableEntity.h"
#include "domain/models/SysRoleDept.h"

/**
 * @brief 角色部门关联实体
 */
class RoleDept : public ChangeableEntity
{
    using SysRoleDept = drogon_model::drogon_admin_db::SysRoleDept;

    std::optional<std::int32_t> roleId_;        ///< 角色id
    std::int32_t deptId_;                       ///< 部门id
    std::optional<std::int32_t> createdBy_;     ///< 创建者id
    std::optional<trantor::Date> createdTime_;  ///< 创建时间

  public:
    RoleDept(const std::int32_t deptId);
    RoleDept(const std::int32_t deptId, int32_t createdBy);

    // 和model类互转
    explicit RoleDept(const SysRoleDept &sysRoleDept);
    explicit operator SysRoleDept() const;

    OPT_SETTER(roleId, RoleId);
    OPT_SETTER(createdBy, CreatedBy);
    OPT_SETTER(createdTime, CreatedTime);

    OPT_GETTER(roleId, RoleId);
    OPT_GETTER(deptId, DeptId);
    OPT_GETTER(createdBy, CreatedBy);
    OPT_GETTER(createdTime, CreatedTime);
};
