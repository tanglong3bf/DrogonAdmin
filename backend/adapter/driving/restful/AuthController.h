#pragma once

#include <drogon/HttpController.h>
#include "application/AuthService.h"
#include "application/LoginRequest.h"

using namespace drogon;

class AuthController : public drogon::HttpController<AuthController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(AuthController::login, "/login", Post, Options);
    METHOD_LIST_END

    /**
     * @brief 用户登录接口
     * @param request LoginRequest 登录请求数据
     * @return HttpResponsePtr json格式的响应体，内部数据为LoginResponse
     *
     * @see LoginRequest
     * @see LoginResponse
     * @see AuthService::login
     */
    Task<HttpResponsePtr> login(const HttpRequestPtr,
                                const LoginRequest request) const
        noexcept(false);

  private:
    AuthServicePtr authService_;
};
