#pragma once

#include "domain/models/SysUserRole.h"
#include "common/framework/domain/ChangeableEntity.h"
#include "common/util/Utilities.hpp"

/**
 * @brief 用户角色关联实体
 */
class UserRole : public ChangeableEntity
{
    using SysUserRole = drogon_model::drogon_admin_db::SysUserRole;

  public:
    UserRole(const std::int32_t roleId);
    UserRole(const std::int32_t roleId, int32_t createdBy);

    /// @group 和model类互转
    /// @{
    explicit UserRole(const SysUserRole &model);
    explicit operator SysUserRole() const;
    /// @}

    GETTER(roleId)

  public:
    // 创建用户时可能没有id
    std::optional<std::int32_t> userId;  ///< 用户id
  private:
    std::int32_t roleId_;  ///< 角色id
  public:
    std::optional<std::int32_t> createdBy;
    std::optional<trantor::Date> createdTime;
};
