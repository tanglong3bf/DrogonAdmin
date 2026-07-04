#pragma once

#include "domain/org/user/UserRole.h"
#include <cstdint>

/**
 * @brief 用户角色响应类
 *
 * @see UserResponse
 */
struct UserRoleResponse
{
    UserRoleResponse(const UserRole &entity)
        : userId_{*entity.userId()}, roleId_{entity.roleId()}
    {
    }

    Json::Value toJson() const
    {
        Json::Value json;
        json["user_id"] = userId_;
        json["role_id"] = roleId_;
        return json;
    }

  private:
    std::int32_t userId_;  ///< 用户ID
    std::int32_t roleId_;  ///< 角色ID
};
