#pragma once

#include "LoginRequest.h"
#include "LoginResponse.h"
#include "CaptchaResponse.h"
#include "domain/cache/AuthCacheRepo.h"
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

    /**
     * @brief 生成验证码图片
     *
     * @param width 图片宽度
     * @param height 图片高度
     * @param font_path 字体路径
     * @return CaptchaResponse
     */
    CaptchaResponse renderCaptchaPng(int width,
                                     int height,
                                     const std::string &font_path);

  private:
    /**
     * @brief 生成验证码文本
     *
     * @param len 长度
     * @return std::string
     */
    std::string genCaptchaText(int len = 4);

  private:
    UserRepositoryPtr userRepository_{
        drogon::DrClassMap::getSingleInstance<UserRepository>()};
    AuthCacheRepoPtr authCacheRepo_{
        drogon::DrClassMap::getSingleInstance<AuthCacheRepoBase>()};
};

using AuthServicePtr = std::shared_ptr<AuthService>;
