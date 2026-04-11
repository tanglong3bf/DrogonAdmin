#pragma once

#include <cstdint>
#include <string>
#include "common/util/Utilities.hpp"

/**
 * @brief 用户状态
 */
enum class Status : int16_t
{
    Enabled = 0,   ///< 正常
    Disabled = 1,  ///< 禁用
};

template <>
std::string toString(const Status &status);
