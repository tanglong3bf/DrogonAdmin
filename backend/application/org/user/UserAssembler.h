#pragma once

#include "dto/UserCreateRequest.h"
#include "domain/org/user/User.h"
#include "domain/org/user/UserVerifier.h"
#include "domain/org/dept/DeptVerifier.h"
#include "domain/iam/role/RoleVerifier.h"
#include "common/framework/DrAdminObject.hpp"
#include <memory>

/**
 * @brief 用户组装器
 */
class UserAssembler : public DrAdminObject<UserAssembler>
{
  public:
    /**
     * @brief 从 UserCreateRequest 创建 User
     */
    drogon::Task<User> fromCreateRequest(const UserCreateRequest &request,
                                         const int32_t createdBy) const;

  private:
    UserVerifierPtr userVerifier_{
        drogon::DrClassMap::getSingleInstance<UserVerifier>()};
    DeptVerifierPtr deptVerifier_{
        drogon::DrClassMap::getSingleInstance<DeptVerifier>()};
    RoleVerifierPtr roleVerifier_{
        drogon::DrClassMap::getSingleInstance<RoleVerifier>()};
};

using UserAssemblerPtr = std::shared_ptr<UserAssembler>;
