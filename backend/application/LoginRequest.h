#pragma once

#include <jsoncpp/json/value.h>
#include <string>

#include "common/util/Utilities.hpp"

/**
 * @brief 用户登录请求数据封装类
 *
 * @see AuthService::login
 */
class LoginRequest
{
  private:
    std::string username_;  ///< 用户名
    std::string password_;  ///< 密码

  public:
    LoginRequest() = default;

    /**
     * @brief 通过json格式请求体设置每一个字段
     *
     * @see drogon::fromRequest<LoginRequest>
     * @see drogon_admin::util::getParam
     */
    void setByJson(const Json::Value &json);

    // getters
    GETTER(username, Username)
    GETTER(password, Password)
};
