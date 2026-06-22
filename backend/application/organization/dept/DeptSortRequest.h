#pragma once

#include "common/util/Utilities.hpp"
#include <jsoncpp/json/value.h>
#include <optional>

/**
 * @brief 更新部门参数
 */
struct DeptSortRequest
{
    DeptSortRequest() = default;
    DeptSortRequest(const Json::Value &json);

    GETTER(parentId)
    GETTER(deptIds)

  private:
    std::optional<std::int32_t> parentId_;  ///< 父部门id
    std::vector<std::int32_t> deptIds_;     ///< 部门id
};
