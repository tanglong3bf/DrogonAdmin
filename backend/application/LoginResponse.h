#pragma once

#include <string>
#include <jsoncpp/json/value.h>

/**
 * @brief 用户登录响应数据封装类
 *
 * @see AuthService::login
 */
class LoginResponse
{
  public:
    LoginResponse(const std::string &token) : token_{token}
    {
    }

    Json::Value toJson() const;

  private:
    std::string token_;  ///< token
};
