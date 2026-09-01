#pragma once

#include "dto/AvatarFileData.h"
#include "dto/ChangePasswordRequest.h"
#include "dto/UploadAvatarResponse.h"
#include "dto/UserInfoUpdateRequest.h"
#include "UserUpdater.h"
#include "domain/org/user/AvatarStorage.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/utils/coroutine.h>

class UserCenterService : public DrAdminObject<UserCenterService>
{
  public:
    /**
     * @brief 更新用户基本信息
     */
    drogon::Task<> updateBasicInfo(std::int32_t userId,
                                   const UserInfoUpdateRequest &request) const;

    /**
     * @brief 更新用户密码
     */
    drogon::Task<> changePassword(std::int32_t userId,
                                  const ChangePasswordRequest &request) const;

    /**
     * @brief 上传用户头像
     */
    drogon::Task<UploadAvatarResponse> uploadAvatar(
        std::int32_t userId,
        const AvatarFileData &fileData) const;

  private:
    UserUpdaterPtr userUpdater_{
        drogon::DrClassMap::getSingleInstance<UserUpdater>()};
    UserRepositoryPtr userRepository_{
        drogon::DrClassMap::getSingleInstance<UserRepository>()};
    AvatarStoragePtr avatarStorage_{
        drogon::DrClassMap::getSingleInstance<AvatarStorage>()};
};

using UserCenterServicePtr = std::shared_ptr<UserCenterService>;
