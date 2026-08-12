#pragma once

#include <cstdint>
#include <string>

/**
 * @brief 用户状态
 */
enum class Status : int16_t
{
    Enabled = 0,   ///< 正常
    Disabled = 1,  ///< 禁用
};

std::string toString(const Status &status);
