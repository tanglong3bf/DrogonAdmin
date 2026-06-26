#include "UserCenterService.h"

using namespace std;
using namespace drogon;

Task<> UserCenterService::updateBasicInfo(
    const std::int32_t userId,
    const UserInfoUpdateRequest &request) const
{
    auto user = co_await userRepository_->getById(userId, true);
    if (user == nullopt)
    {
        throw BusinessException("用户不存在");
    }
    co_await userUpdater_->updateBasicInfo(*user, request);
    co_await userRepository_->save(*user);
}
