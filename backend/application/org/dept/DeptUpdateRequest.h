#pragma once

#include "common/util/Utilities.hpp"
#include <jsoncpp/json/value.h>
#include <string>

/**
 * @brief 更新部门参数
 */
struct DeptUpdateRequest
{
    DeptUpdateRequest() = default;
    DeptUpdateRequest(const Json::Value &json);

    std::string_view name() const
    {
        return name_;
    }

    GETTER(version);

    std::string name_;      ///< 部门名称
    std::int32_t version_;  ///< 乐观锁版本号
};
