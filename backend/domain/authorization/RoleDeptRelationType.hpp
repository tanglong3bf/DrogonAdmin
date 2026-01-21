#pragma once

#include <cstdint>

/**
 * @brief 角色部门关联关系
 */
enum class RoleDeptRelationType : std::int16_t
{
    BELONG,  ///< 角色属于指定部门
    EXCLUDE  ///< 角色排除指定部门
};
