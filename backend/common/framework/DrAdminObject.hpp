#pragma once

#include <drogon/DrObject.h>

/**
 * @brief 非controller 非builder 的公共基类
 */
template <typename T, bool AutoCreation = true>
struct DrAdminObject : public drogon::DrObject<T>
{
    static constexpr bool isAutoCreation = AutoCreation;
};
