#include "AuthService.h"

#include "DrogonJwtUtil/src/JwtUtil.h"
#include "common/exception/BusinessException.h"

using namespace std;
using namespace drogon;

bool matches(string_view raw, string_view mask);

Task<LoginResponse> AuthService::login(const LoginRequest &request) const
{
    const auto user =
        co_await userRepository_->getByUsername(request.username(), true);
    if (!user)
    {
        throw BusinessException{"用户不存在，登录失败"};
    }

    if (!matches(request.password(), user->password))
    {
        throw BusinessException{"密码错误，登录失败"};
    }

    static auto *jwtUtil = drogon::app().getPlugin<tl::jwt::JwtUtil>();

    // 暂时只存userId，后续会存权限
    Json::Value jwtData;
    jwtData["user_id"] = *user->userId;
    const auto token = jwtUtil->encode(jwtData);
    co_return LoginResponse{token, UserResponse{*user}};
}
