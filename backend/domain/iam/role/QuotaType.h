#pragma once

#include <cstdint>
#include <string>

/**
 * @brief 用户数量限制类型
 */
enum class QuotaType : int16_t
{
    Unlimited = 0,    ///< 不限制
    TotalLimit = 1,   ///< 总数量限制
    PerDeptLimit = 2  ///< 每个部门用户数量限制
};

std::string toString(const QuotaType &quotaType);

// vim: set filetype = cpp:
