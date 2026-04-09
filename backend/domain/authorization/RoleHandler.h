#pragma once

#include <memory>
#include "common/framework/DrAdminObject.hpp"
#include "domain/authorization/RoleRepository.h"
#include "domain/organization/user/UserVerifier.h"

class RoleHandler : public DrAdminObject<RoleHandler>
{
  public:
    /**
     * @brief 删除角色
     */
    drogon::Task<> deleteRole(Role &role, int32_t deletedBy) const;

  private:
    RoleRepositoryPtr roleRepository_{
        drogon::DrClassMap::getSingleInstance<RoleRepository>()};
    UserVerifierPtr userVerifier_{
        drogon::DrClassMap::getSingleInstance<UserVerifier>()};
};

using RoleHandlerPtr = std::shared_ptr<RoleHandler>;
