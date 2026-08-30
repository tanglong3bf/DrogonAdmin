#pragma once

#include "UserRoleResponse.h"
#include "domain/org/user/Sex.h"
#include "domain/org/user/Status.h"
#include "domain/org/user/User.h"
#include <optional>
#include <string>
#include <cstdint>

/**
 * @brief 用户响应类
 */
struct UserResponse
{
    explicit UserResponse(const User &user);

    Json::Value toJson() const;

  private:
    std::int32_t userId_;                      ///< 用户id
    std::string username_;                     ///< 用户名
    std::string nickname_;                     ///< 昵称
    std::string avatar_;                       ///< 头像
    Sex sex_;                                  ///< 性别
    std::int32_t deptId_;                      ///< 部门
    std::optional<PhoneNumber> phoneNumber_;   ///< 电话号码
    std::optional<Email> email_;               ///< 邮箱
    Status status_;                            ///< 状态
    std::int32_t version_;                     ///< 乐观锁版本号
    std::vector<UserRoleResponse> userRoles_;  ///< 拥有的角色
};
