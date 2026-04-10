#pragma once

#include <jsoncpp/json/value.h>
#include <optional>
#include <string>
#include <vector>
#include "domain/organization/user/Sex.h"
#include "domain/organization/user/Status.h"

class UserUpdateRequest
{
    std::optional<std::string> nickname_;
    std::optional<Sex> sex_;
    std::optional<std::int32_t> deptId_;
    std::optional<std::string> phoneNumber_;
    std::optional<std::string> email_;
    std::optional<Status> status_;
    std::optional<std::vector<std::int32_t>> roleIds_;

  public:
    void setByJson(const Json::Value &json);

    OPT_GETTER(nickname, Nickname)
    OPT_GETTER(sex, Sex)
    OPT_GETTER(deptId, DeptId)
    OPT_GETTER(phoneNumber, PhoneNumber)
    OPT_GETTER(email, Email)
    OPT_GETTER(status, Status)
    GETTER(roleIds, RoleIds)
};
