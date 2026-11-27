#pragma once

#include <string>
#include <utility>
#include <vector>
#include "application/iam/menu/MenuResponse.h"
#include "application/org/user/dto/UserResponse.h"
#include <jsoncpp/json/value.h>

/**
 * @brief 用户登录响应数据封装类
 *
 * @see AuthService::login
 */
class LoginResponse
{
  public:
    LoginResponse(std::string_view token,
                  const UserResponse &user,
                  std::vector<MenuResponse> menuList)
        : token_{token}, user_{user}, menuList_{std::move(menuList)}
    {
    }

    Json::Value toJson() const;

  private:
    std::string token_;                   ///< token
    UserResponse user_;                   ///< 用户
    std::vector<MenuResponse> menuList_;  ///< 可访问的菜单树
};
