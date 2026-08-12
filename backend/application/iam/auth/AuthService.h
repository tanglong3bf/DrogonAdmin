#pragma once

#include "LoginRequest.h"
#include "LoginResponse.h"
#include "domain/org/user/UserRepository.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/DrClassMap.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/utils/coroutine.h>
#include <memory>

class AuthService : public DrAdminObject<AuthService>
{
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

  private:
    UserRepositoryPtr userRepository_;
};

using AuthServicePtr = std::shared_ptr<AuthService>;
