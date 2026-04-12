#pragma once

#include "UserRoleResponse.h"
#include "domain/organization/user/Sex.h"
#include "domain/organization/user/Status.h"
#include "domain/organization/user/User.h"
#include <optional>
#include <string>
#include <cstdint>

/**
 * @brief 用户响应类
 */
class UserResponse
{
    std::int32_t userId_;                      ///< 用户id
    std::string username_;                     ///< 用户名
    std::string nickname_;                     ///< 昵称
    std::string avatar_;                       ///< 头像
    Sex sex_;                                  ///< 性别
    std::int32_t deptId_;                      ///< 部门
    std::optional<PhoneNumber> phoneNumber_;   ///< 电话号码
    std::optional<Email> email_;               ///< 邮箱
    Status status_;                            ///< 状态
    std::vector<UserRoleResponse> userRoles_;  ///< 拥有的角色

  public:
    // 实体类
    explicit UserResponse(const User &user);

    Json::Value toJson() const;

    GETTER(userId, UserId)
    GETTER(username, Username)
    GETTER(nickname, Nickname)
    GETTER(avatar, Avatar)
    GETTER(sex, Sex)
    GETTER(deptId, DeptId)
    OPT_GETTER(phoneNumber, PhoneNumber)
    OPT_GETTER(email, Email)
    GETTER(status, Status)
};
