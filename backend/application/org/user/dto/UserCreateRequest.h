#pragma once

#include "domain/org/user/Email.h"
#include "domain/org/user/PhoneNumber.h"
#include "domain/org/user/Sex.h"
#include "domain/org/user/Status.h"
#include "common/util/Utilities.hpp"
#include <jsoncpp/json/value.h>
#include <optional>
#include <cstdint>

/**
 * @brief 创建用户请求
 *
 * @note 密码默认为 123456
 * @note 头像默认为 #
 */
struct UserCreateRequest
{
    UserCreateRequest() = default;
    UserCreateRequest(const Json::Value &json);

    std::string_view username() const
    {
        return username_;
    }

    std::string_view nickname() const
    {
        return nickname_;
    }

    GETTER(sex)
    GETTER(deptId)
    GETTER(phoneNumber)
    GETTER(email)
    GETTER(status)
    GETTER(roleIds)

  private:
    std::string username_;                              ///< 用户名
    std::string nickname_;                              ///< 昵称
    Sex sex_;                                           ///< 性别
    std::int32_t deptId_;                               ///< 所属部门
    std::optional<PhoneNumber> phoneNumber_;            ///< 手机号
    std::optional<Email> email_;                        ///< 邮箱
    Status status_;                                     ///< 状态
    std::optional<std::vector<std::int32_t>> roleIds_;  ///< 拥有的角色
};
