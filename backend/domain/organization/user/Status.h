#pragma once

#include <cstdint>
#include <string>
#include "common/util/Utilities.hpp"

enum class Status : int8_t
{
    Enabled = 0,
    Disabled = 1,
};

template <>
std::string toString(const Status &status);
