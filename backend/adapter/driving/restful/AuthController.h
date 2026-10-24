#pragma once

#include "application/iam/auth/AuthService.h"
#include "application/iam/auth/LoginRequest.h"
#include <drogon/HttpController.h>

using namespace drogon;

class AuthController : public drogon::HttpController<AuthController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(AuthController::login, "/login", Post, Options);
    ADD_METHOD_TO(AuthController::getCaptcha,
                  "/captcha?old_captcha_id={}",
                  Get,
                  Options);
    METHOD_LIST_END

    /**
     * @brief 用户登录接口
     * @param request LoginRequest 登录请求数据
     * @return HttpResponsePtr json格式的响应体，内部数据为 LoginResponse
     *
     * @see LoginRequest
     * @see LoginResponse
     * @see AuthService::login
     */
    Task<HttpResponsePtr> login(const HttpRequestPtr,
                                const LoginRequest request) const;

    /**
     * @brief 获取验证码接口
     * @param oldCaptchaId 旧的验证码id，用于清除redis缓存
     * @return HttpResponsePtr 验证码id存放于响应头X-Captcha-Id中，响应体为 png
     * 格式的图片
     */
    Task<HttpResponsePtr> getCaptcha(const HttpRequestPtr,
                                     const std::string_view oldCaptchaId) const;

  private:
    AuthServicePtr authService_{
        drogon::DrClassMap::getSingleInstance<AuthService>()};
    AuthCacheRepoPtr authCacheRepo_{
        drogon::DrClassMap::getSingleInstance<AuthCacheRepoBase>()};
};
