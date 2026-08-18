#pragma once

#include <jsoncpp/json/value.h>
#include <string>
#include <cstdint>

/**
 * 可分配角色
 */
struct AssignableRoleResponse
{
    AssignableRoleResponse(std::int32_t roleId,
                           const std::string &name,
                           std::int32_t version);
    Json::Value toJson() const;

  private:
    const std::int32_t roleId_;   ///< 角色ID
    const std::string name_;      ///< 角色名称
    const std::int32_t version_;  ///< 乐观锁版本号
};
