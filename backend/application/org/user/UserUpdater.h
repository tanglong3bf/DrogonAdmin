#pragma once

#include "dto/UserUpdateRequest.h"
#include "domain/org/user/User.h"
#include "domain/org/dept/DeptVerifier.h"
#include "domain/iam/role/RoleVerifier.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/utils/coroutine.h>

/**
 * @brief 用户更新服务
 */
class UserUpdater : public DrAdminObject<UserUpdater>
{
  public:
    /**
     * @brief 更新用户
     */
    drogon::Task<> updateUser(User &user,
                              const UserUpdateRequest &request,
                              const std::int32_t updatedBy) const;

  private:
    DeptVerifierPtr deptVerifier_{
        drogon::DrClassMap::getSingleInstance<DeptVerifier>()};
    RoleVerifierPtr roleVerifier_{
        drogon::DrClassMap::getSingleInstance<RoleVerifier>()};
};

using UserUpdaterPtr = std::shared_ptr<UserUpdater>;
