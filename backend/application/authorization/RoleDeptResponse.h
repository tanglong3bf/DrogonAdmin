#pragma once

#include <jsoncpp/json/value.h>
#include <cstdint>
#include "domain/authorization/RoleDept.h"

class RoleDeptResponse
{
    std::int32_t id_;
    std::int32_t roleId_;
    std::int32_t deptId_;

  public:
    RoleDeptResponse(const RoleDept &roleDept);

    Json::Value toJson() const;
};
