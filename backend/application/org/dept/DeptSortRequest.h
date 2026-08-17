#pragma once

#include "common/util/Utilities.hpp"
#include <jsoncpp/json/value.h>
#include <optional>

struct DeptSortItem
{
    std::int32_t deptId;
    std::int32_t sortNum;
    std::int32_t version;
};

/**
 * @brief 更新部门参数
 */
struct DeptSortRequest
{
    DeptSortRequest() = default;
    DeptSortRequest(const Json::Value &json);

    GETTER(parentId)
    GETTER(depts)

    std::vector<std::int32_t> deptIds() const;

  private:
    std::optional<std::int32_t> parentId_;  ///< 父部门id
    std::vector<DeptSortItem> depts_;       ///< 部门id
};
