#pragma once

#include <cstdint>
#include "common/util/Utilities.hpp"

/**
 * @brief 性别
 */
enum class Sex : std::int16_t
{
    Unknown = 0,
    Male = 1,
    Female = 2,
};

template <>
std::string toString(const Sex &sex);
