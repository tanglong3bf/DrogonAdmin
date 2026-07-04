#pragma once

#include <cstdint>
#include <string>

/**
 * @brief 性别
 */
enum class Sex : std::int16_t
{
    Unknown = 0,
    Male = 1,
    Female = 2,
};

std::string toString(const Sex &sex);
