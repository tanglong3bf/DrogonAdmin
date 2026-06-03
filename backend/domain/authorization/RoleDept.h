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

  public:
    RoleDept(const std::int32_t deptId);
    RoleDept(const std::int32_t deptId, int32_t createdBy);

    // 和model类互转
    explicit RoleDept(const SysRoleDept &sysRoleDept);
    explicit operator SysRoleDept() const;

    GETTER(deptId)

    std::optional<std::int32_t> roleId;  ///< 角色id
  private:
    std::int32_t deptId_;  ///< 部门id
  public:
    std::optional<std::int32_t> createdBy;     ///< 创建者id
    std::optional<trantor::Date> createdTime;  ///< 创建时间
};
