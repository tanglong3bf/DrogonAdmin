#pragma once

#include <memory>
#include <drogon/utils/coroutine.h>
#include "common/framework/DrAdminObject.hpp"
#include "UserRepository.h"

/**
 * @brief 用户校验器
 */
class UserVerifier : public DrAdminObject<UserVerifier>
{
  public:
    /**
     * 验证没有用户属于指定部门
     */
    drogon::Task<> verifyNoUserInDept(const std::int32_t deptId) const;

  private:
    UserRepositoryPtr userRepository_{
        drogon::DrClassMap::getSingleInstance<UserRepository>()};
};

using UserVerifierPtr = std::shared_ptr<UserVerifier>;
