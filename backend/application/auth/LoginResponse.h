#pragma once

#include <string>
#include "application/organization/user/dto/UserResponse.h"
#include <jsoncpp/json/value.h>

/**
 * @brief 用户登录响应数据封装类
 *
 * @see AuthService::login
 */
class LoginResponse
{
  public:
    LoginResponse(std::string_view token, const UserResponse &user)
        : token_{token}, user_{user}
    {
    }

    Json::Value toJson() const;

  private:
    std::string token_;  ///< token
    UserResponse user_;  ///< 用户
};
