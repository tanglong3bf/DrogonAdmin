#pragma once

#include <cstdint>
#include <string>

/**
 * @brief 角色部门关联类型
 */
enum class RelationType : std::int16_t
{
    All = 0,        ///< 所有部门可用角色
    Whitelist = 1,  ///< 白名单
    Blacklist = 2   ///< 黑名单
};

std::string toString(const RelationType &relationType);

// vim: set filetype = cpp:
