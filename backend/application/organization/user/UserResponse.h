#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include "domain/organization/Sex.h"
#include "domain/organization/Status.h"
#include "domain/organization/User.h"

class UserResponse
{
    std::int32_t userId_;
    std::string username_;
    std::string nickname_;
    std::string avatar_;
    Sex sex_;
    std::int32_t deptId_;
    std::optional<std::string> phoneNumber_;
    std::optional<std::string> email_;
    Status status_;

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
