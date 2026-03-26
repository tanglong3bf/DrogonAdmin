#pragma once

#include <drogon/orm/Result.h>
#include <jsoncpp/json/value.h>
#include "domain/authorization/Role.h"

class RoleResponse
{
    std::int32_t roleId_;
    std::string name_;
    std::string code_;
    std::optional<std::string> description_;
    std::optional<std::int32_t> userQuota_;
    QuotaType quotaType_;

  public:
    RoleResponse(const Role &role);

  public:
    Json::Value toJson() const;
};
