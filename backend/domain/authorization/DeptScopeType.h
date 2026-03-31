#pragma once

#include <cstdint>
#include "common/util/Utilities.hpp"

/**
 * @brief 角色部门关联类型
 */
enum class RelationType : std::int8_t
{
    All = 0,        ///< 所有部门可用角色
    Whitelist = 1,  ///< 白名单
    Blacklist = 2   ///< 黑名单
};

template <>
std::string toString(const RelationType &deptScopeType);
