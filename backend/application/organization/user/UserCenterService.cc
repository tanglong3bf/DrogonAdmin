#include "UserCenterService.h"

using namespace std;
using namespace drogon;

Task<> UserCenterService::updateBasicInfo(
    const std::int32_t userId,
    const UserInfoUpdateRequest &request) const
{
    // userId源自于jwt，可保证有数据
    auto user = co_await userRepository_->getById(userId, true);

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
    user->updatePassword(request.oldPassword(), request.newPassword());
    co_await userRepository_->save(*user);
}

Task<UploadAvatarResponse> UserCenterService::uploadAvatar(
    const std::int32_t userId,
    const HttpRequestPtr &req) const
{
    auto user = co_await userRepository_->getById(userId);
    const string avatar_path = co_await user->updateAvatar(req);
    co_await userRepository_->save(*user);
    co_return UploadAvatarResponse{avatar_path};
}
