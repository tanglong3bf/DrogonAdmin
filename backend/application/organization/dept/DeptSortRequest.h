#pragma once

/**
 * @brief 更新部门参数
 */
#include "common/util/Utilities.hpp"
#include <jsoncpp/json/value.h>

class DeptSortRequest
{
    std::optional<std::int32_t> parentId_;  ///< 父部门id
    std::vector<std::int32_t> deptIds_;     ///< 部门id

  public:
    void setByJson(const Json::Value &json);

    // getters
    OPT_GETTER(parentId, ParentId)
    GETTER(deptIds, DeptIds)
};
