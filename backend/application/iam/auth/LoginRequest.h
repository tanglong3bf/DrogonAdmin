#pragma once

#include <string>
#include "common/util/Utilities.hpp"
#include <jsoncpp/json/value.h>

/**
 * @brief 用户登录请求数据封装类
 *
 * @see AuthService::login
 */
class LoginRequest
{
  public:
    LoginRequest() = default;
    /**
     * @brief 通过json格式请求体设置每一个字段
     *
     * @see drogon::fromRequest<LoginRequest>
     * @see drogon_admin::util::getParam
     */
    LoginRequest(const Json::Value &json);

    GETTER_STR_VIEW(username);
    GETTER_STR_VIEW(password);

  private:
    std::string username_;  ///< 用户名
    std::string password_;  ///< 密码
};
