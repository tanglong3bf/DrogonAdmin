#pragma once

#include <cstdint>
#include "domain/organization/user/UserRole.h"

class UserRoleResponse
{
    std::int32_t userId_;
    std::int32_t roleId_;

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
