#pragma once

#include "common/util/Utilities.hpp"
#include <jsoncpp/json/value.h>
#include <optional>
#include <string>
#include <cstdint>

/**
 * @brief 创建部门参数
 */
struct DeptCreateRequest
{
    DeptCreateRequest() = default;
    DeptCreateRequest(const Json::Value &json);

    std::string_view name() const
    {
        return name_;
    }

    GETTER(parentId)

  private:
    std::string name_;                      ///< 部门名称
    std::optional<std::int32_t> parentId_;  ///< 父部门ID
};
