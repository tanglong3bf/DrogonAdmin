#pragma once

#include "application/organization/user/dto/ChangePasswordRequest.h"
#include "dto/UserInfoUpdateRequest.h"
#include "UserUpdater.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/utils/coroutine.h>

class UserCenterService : public DrAdminObject<UserCenterService>
{
  public:
    /**
     * @brief 更新用户基本信息
     */
    drogon::Task<> updateBasicInfo(const std::int32_t userId,
                                   const UserInfoUpdateRequest &request) const;

    drogon::Task<> changePassword(const std::int32_t userId,
                                  const ChangePasswordRequest &request) const;

  private:
    UserUpdaterPtr userUpdater_{
        drogon::DrClassMap::getSingleInstance<UserUpdater>()};
    UserRepositoryPtr userRepository_{
        drogon::DrClassMap::getSingleInstance<UserRepository>()};
};

using UserCenterServicePtr = std::shared_ptr<UserCenterService>;
