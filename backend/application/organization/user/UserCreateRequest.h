#pragma once

#include <cstdint>
#include <optional>
#include <jsoncpp/json/value.h>
#include "common/util/Utilities.hpp"
#include "domain/organization/user/Sex.h"
#include "domain/organization/user/Status.h"

/**
 * @brief 创建用户请求
 *
 * @note 密码默认为 123456
 * @note 头像默认为 #
 */
class UserCreateRequest
{
    std::string username_;
    std::string nickname_;
    Sex sex_;
    std::int32_t deptId_;
    std::optional<std::string> phoneNumber_;
    std::optional<std::string> email_;
    Status status_;
    std::optional<std::vector<std::int32_t>> roleIds_;

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
