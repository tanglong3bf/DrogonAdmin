#pragma once

#include "domain/organization/user/Email.h"
#include "domain/organization/user/PhoneNumber.h"
#include "domain/organization/user/Sex.h"
#include "domain/organization/user/Status.h"
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
class UserCreateRequest
{
    std::string username_;                              ///< 用户名
    std::string nickname_;                              ///< 昵称
    Sex sex_;                                           ///< 性别
    std::int32_t deptId_;                               ///< 所属部门
    std::optional<PhoneNumber> phoneNumber_;            ///< 手机号
    std::optional<Email> email_;                        ///< 邮箱
    Status status_;                                     ///< 状态
    std::optional<std::vector<std::int32_t>> roleIds_;  ///< 拥有的角色

  public:
    void setByJson(const Json::Value &json);

    // getters
    GETTER(username, Username)
    GETTER(nickname, Nickname)
    GETTER(sex, Sex)
    GETTER(deptId, DeptId)
    OPT_GETTER(phoneNumber, PhoneNumber)
    OPT_GETTER(email, Email)
    GETTER(status, Status)
    OPT_GETTER(roleIds, RoleIds)
};
