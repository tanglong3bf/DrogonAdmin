#pragma once

#include <memory>
#include <drogon/HttpAppFramework.h>
#include <drogon/utils/coroutine.h>
#include "common/framework/DrAdminObject.hpp"
#include "LoginRequest.h"
#include "LoginResponse.h"

class AuthService : public DrAdminObject<AuthService>
{
  public:
    AuthService() = default;

  public:
    /**
     * @brief 处理登录请求
     *
     * @see AuthController::login
     * @see LoginRequest
     * @see LoginResponse
     * @see tl::jwt::JwtUtil
     */
    drogon::Task<LoginResponse> login(const LoginRequest &request) const
        noexcept(false);
};

using AuthServicePtr = std::shared_ptr<AuthService>;
