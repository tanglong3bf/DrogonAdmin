#pragma once

#include "domain/authorization/RoleDept.h"
#include <jsoncpp/json/value.h>
#include <cstdint>

class RoleDeptResponse
{
    std::int32_t roleId_;
    std::int32_t deptId_;

  public:
    RoleDeptResponse(const RoleDept &roleDept);

    Json::Value toJson() const;
};
