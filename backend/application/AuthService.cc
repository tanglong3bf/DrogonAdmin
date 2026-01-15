#include "AuthService.h"

#include "DrogonJwtUtil/src/JwtUtil.h"

using namespace drogon;

Task<LoginResponse> AuthService::login(const LoginRequest & /*ignore*/) const
{
    // 忽略所有输入，给你一个jwt
    static auto *jwtUtil = drogon::app().getPlugin<tl::jwt::JwtUtil>();

    Json::Value jwtData;
    jwtData["user_id"] = 1;
    const auto token = jwtUtil->encode(jwtData);
    co_return LoginResponse{token};
}
