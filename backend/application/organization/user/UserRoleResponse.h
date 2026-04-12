#pragma once

#include "domain/organization/user/UserRole.h"
#include <cstdint>

/**
 * @brief 用户角色响应类
 *
 * @see UserResponse
 */
class UserRoleResponse
{
    std::int32_t userId_;  ///< 用户ID
    std::int32_t roleId_;  ///< 角色ID

  public:
    UserRoleResponse(const UserRole &entity)
        : userId_{*entity.getUserId()}, roleId_{entity.getRoleId()}
    {
    }

    Json::Value toJson() const
    {
        Json::Value json;
        json["user_id"] = userId_;
        json["role_id"] = roleId_;
        return json;
    }
};
