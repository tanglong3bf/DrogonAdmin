#pragma once

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

    std::string name_;  ///< 部门名称
};
