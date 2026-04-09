#pragma once

#include <memory>
#include "application/organization/user/UserCreateRequest.h"
#include "common/framework/DrAdminObject.hpp"
#include "domain/organization/user/User.h"
#include "domain/organization/user/UserVerifier.h"
#include "domain/organization/dept/DeptVerifier.h"

class UserAssembler : public DrAdminObject<UserAssembler>
{
  public:
    drogon::Task<User> fromCreateRequest(const UserCreateRequest &request,
                                         const int32_t createdBy) const;

  private:
    UserVerifierPtr userVerifier_{
        drogon::DrClassMap::getSingleInstance<UserVerifier>()};
    DeptVerifierPtr deptVerifier_{
        drogon::DrClassMap::getSingleInstance<DeptVerifier>()};
};

using UserAssemblerPtr = std::shared_ptr<UserAssembler>;
