#include "UserCenterService.h"

using namespace std;
using namespace drogon;

Task<> UserCenterService::updateBasicInfo(
    const std::int32_t userId,
    const UserInfoUpdateRequest &request) const
{
    // userId源自于jwt，可保证有数据
    auto user = co_await userRepository_->getById(userId, true);
    if (user->version() != request.version())
    {
        throw BusinessException("更新期间数据发生变化，更新失败");
    }

    const bool isUpdated = user->updateBasicInfo(request.nickname(),
                                                 request.sex(),
                                                 request.phoneNumber(),
                                                 request.email());
    if (isUpdated)
    {
        co_await userRepository_->save(*user);
    }
}

Task<> UserCenterService::changePassword(
    const std::int32_t userId,
    const ChangePasswordRequest &request) const
{
    // userId源自于jwt，可保证有数据
    auto user = co_await userRepository_->getById(userId, true);
    if (user->version() != request.version())
    {
        throw BusinessException("更新期间数据发生变化，更新失败");
    }
    user->updatePassword(request.oldPassword(), request.newPassword());
    co_await userRepository_->updatePassword(*user);
}

Task<UploadAvatarResponse> UserCenterService::uploadAvatar(
    const std::int32_t userId,
    const AvatarFileData &file) const
{
    auto user = co_await userRepository_->getById(userId);
    auto avatar_path = co_await avatarStorage_->saveAvatar(file);
    user->setAvatar(avatar_path);
    co_await userRepository_->save(*user);
    co_return UploadAvatarResponse{avatar_path};
}
