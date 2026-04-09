#pragma once

#include <jsoncpp/json/value.h>
#include <cstdint>
#include <string>

class AssignableRoleResponse
{
    std::int32_t roleId_;
    std::string name_;

  public:
    AssignableRoleResponse(const std::int32_t roleId,
                           const std::string &name);
    Json::Value toJson() const;
};
