#pragma once

#include "Role.h"
#include "domain/organization/user/UserVerifier.h"
#include "common/framework/DrAdminObject.hpp"
#include <memory>

class RoleHandler : public DrAdminObject<RoleHandler>
{
  public:
    /**
     * @brief 删除角色
     */
    drogon::Task<> deleteRole(Role &role, int32_t deletedBy) const;

  private:
    UserVerifierPtr userVerifier_{
        drogon::DrClassMap::getSingleInstance<UserVerifier>()};
};

using RoleHandlerPtr = std::shared_ptr<RoleHandler>;
