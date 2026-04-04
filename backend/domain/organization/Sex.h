#pragma once

#include <cstdint>
#include "common/util/Utilities.hpp"

enum class Sex : int8_t
{
    Unknown = 0,
    Male = 1,
    Female = 2,
};

template <>
std::string toString(const Sex &sex);
