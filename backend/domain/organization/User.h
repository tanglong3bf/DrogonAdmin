#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include "Sex.h"
#include "Status.h"
#include "common/framework/domain/AuditableEntity.h"
#include "common/framework/domain/ChangeableEntity.h"
#include "common/util/Utilities.hpp"
#include "domain/models/SysUser.h"

class User : public AuditableEntity, public ChangeableEntity
{
    using SysUser = drogon_model::drogon_admin_db::SysUser;

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
    // 与模型类互转
    explicit User(const SysUser &sysUser);
    explicit operator SysUser() const;

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
