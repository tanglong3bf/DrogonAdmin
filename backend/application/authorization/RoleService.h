#pragma once

#include <drogon/utils/coroutine.h>
#include <memory>
#include "common/framework/DrAdminObject.hpp"
#include "domain/authorization/RoleRepository.h"

class RoleService : public DrAdminObject<RoleService>
{
  public:
    drogon::Task<> deleteExcludingDept(const std::int32_t deptId,
                                       const std::int32_t deletedBy) const;

  private:
    RoleRepositoryPtr roleRepository_{
        drogon::DrClassMap::getSingleInstance<RoleRepository>()};
};

using RoleServicePtr = std::shared_ptr<RoleService>;
