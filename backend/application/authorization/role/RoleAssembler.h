#pragma once

#include "dto/RoleCreateRequest.h"
#include "domain/authorization/Role.h"
#include "domain/authorization/RoleRepository.h"
#include "domain/authorization/RoleVerifier.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/utils/coroutine.h>

/**
 * @brief 角色装配器
 */
class RoleAssembler : public DrAdminObject<RoleAssembler>
{
  public:
    /**
     * @brief 用RoleCreateRequest构建Role对象
     */
    drogon::Task<Role> fromCreateRequest(const RoleCreateRequest request,
                                         std::int32_t createdBy);

  private:
    RoleRepositoryPtr roleRepository_{
        drogon::DrClassMap::getSingleInstance<RoleRepository>()};
    RoleVerifierPtr roleVerifier_{
        drogon::DrClassMap::getSingleInstance<RoleVerifier>()};
};

using RoleAssemblerPtr = std::shared_ptr<RoleAssembler>;
