#pragma once

#include "dto/ChangePasswordRequest.h"
#include "dto/UploadAvatarResponse.h"
#include "dto/UserInfoUpdateRequest.h"
#include "UserUpdater.h"
#include "domain/organization/user/AvatarStorage.h"
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

    /**
     * @brief 更新用户密码
     */
    drogon::Task<> changePassword(const std::int32_t userId,
                                  const ChangePasswordRequest &request) const;

    /**
     * @brief 上传用户头像
     */
    drogon::Task<UploadAvatarResponse> uploadAvatar(
        const std::int32_t userId,
        const AvatarFileData &file) const;

  private:
    UserUpdaterPtr userUpdater_{
        drogon::DrClassMap::getSingleInstance<UserUpdater>()};
    UserRepositoryPtr userRepository_{
        drogon::DrClassMap::getSingleInstance<UserRepository>()};
    AvatarStoragePtr avatarStorage_{
        drogon::DrClassMap::getSingleInstance<AvatarStorage>()};
};

using UserCenterServicePtr = std::shared_ptr<UserCenterService>;
