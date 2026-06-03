#pragma once

#include "domain/authorization/RoleDept.h"
#include <jsoncpp/json/value.h>
#include <cstdint>

struct RoleDeptResponse
{
    RoleDeptResponse(const RoleDept &roleDept);
    Json::Value toJson() const;

  private:
    const std::int32_t roleId_;
    const std::int32_t deptId_;
};
