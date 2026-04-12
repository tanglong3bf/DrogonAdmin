#pragma once

#include <jsoncpp/json/value.h>
#include <string>
#include <cstdint>

/**
 * 可分配角色
 */
class AssignableRoleResponse
{
    std::int32_t roleId_;  ///< 角色ID
    std::string name_;     ///< 角色名称

  public:
    AssignableRoleResponse(const std::int32_t roleId, const std::string &name);
    Json::Value toJson() const;
};
