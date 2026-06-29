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
    friend class User;
    friend class UserRepository;

  public:
    GETTER(userId)
    GETTER(roleId)
    GETTER(createdBy)
    GETTER(createdTime)

  private:
    UserRole(const std::int32_t roleId);
    UserRole(const std::int32_t roleId, std::int32_t createdBy);

    /// @group 和model类互转
    /// @{
  private:
    explicit UserRole(const SysUserRole &model);

  public:
    explicit operator SysUserRole() const;
    /// @}

    // 创建用户时可能没有id
    std::optional<std::int32_t> userId_;  ///< 用户id
    std::int32_t roleId_;                 ///< 角色id
    std::optional<std::int32_t> createdBy_;
    std::optional<trantor::Date> createdTime_;
};
