#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "common/util/Utilities.hpp"
#include "domain/organization/Sex.h"
#include "domain/organization/Status.h"

class UserQueryRequest
{
    std::optional<std::string> username_;
    std::optional<std::string> nickname_;
    std::optional<Sex> sex_;
    std::optional<std::int32_t> deptId_;
    std::optional<std::string> phoneNumber_;
    std::optional<std::string> email_;
    std::optional<Status> status_;
    std::size_t page_;
    std::size_t pageSize_;

  public:
    UserQueryRequest(std::string username,
                     std::string nickname,
                     std::string sex,
                     std::string deptId,
                     std::string phoneNumber,
                     std::string email,
                     std::string status,
                     std::string page,
                     std::string pageSize);

    OPT_GETTER(username, Username)
    OPT_GETTER(nickname, Nickname)
    OPT_GETTER(sex, Sex)
    OPT_GETTER(deptId, DeptId)
    OPT_GETTER(phoneNumber, PhoneNumber)
    OPT_GETTER(email, Email)
    OPT_GETTER(status, Status)
    GETTER(page, Page)
    GETTER(pageSize, PageSize)
};
