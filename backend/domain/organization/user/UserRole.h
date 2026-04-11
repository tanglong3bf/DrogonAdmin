#pragma once

#include "common/util/Utilities.hpp"
#include "common/framework/domain/ChangeableEntity.h"
#include "domain/models/SysUserRole.h"

/**
 * @brief 用户角色关联实体
 */
class UserRole : public ChangeableEntity
{
    using SysUserRole = drogon_model::drogon_admin_db::SysUserRole;

    // 创建用户时可能没有id
    std::optional<std::int32_t> userId_;  ///< 用户id
    std::int32_t roleId_;                 ///< 角色id
    std::optional<std::int32_t> createdBy_;
    std::optional<trantor::Date> createdTime_;

  public:
    UserRole(const std::int32_t roleId);
    UserRole(const std::int32_t roleId, int32_t createdBy);

    // 和model类互转
    explicit UserRole(const SysUserRole &model);
    explicit operator SysUserRole() const;

    OPT_SETTER(userId, UserId);
    OPT_SETTER(createdBy, CreatedBy);
    OPT_SETTER(createdTime, CreatedTime);

    OPT_GETTER(userId, UserId);
    OPT_GETTER(roleId, RoleId);
    OPT_GETTER(createdBy, CreatedBy);
    OPT_GETTER(createdTime, CreatedTime);
};
