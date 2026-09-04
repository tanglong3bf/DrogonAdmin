#include "UserCenterService.h"

using namespace std;
using namespace drogon;

Task<> UserCenterService::updateBasicInfo(
    const int32_t userId,
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
    const int32_t userId,
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
    const int32_t userId,
    const AvatarFileData &fileData) const
{
    auto user = co_await userRepository_->getById(userId);
    const auto avatarFileName = avatarStorage_->saveAvatar(fileData.content,
                                                           fileData.extension,
                                                           fileData.md5);
    const auto &config = app().getCustomConfig();
    const string imgPrefix = config.get("img_prefix", "uploads").asString();
    user->setAvatar(imgPrefix + '/' + avatarFileName);
    co_await userRepository_->save(*user);
    co_return UploadAvatarResponse{imgPrefix + '/' + avatarFileName};
}
