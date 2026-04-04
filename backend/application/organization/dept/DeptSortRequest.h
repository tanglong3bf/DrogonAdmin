#pragma once

/**
 * @brief 更新部门参数
 */
#include <jsoncpp/json/value.h>
#include "common/util/Utilities.hpp"

class DeptSortRequest
{
    std::optional<int32_t> parentId_;
    std::vector<int32_t> deptIds_;

  public:
    void setByJson(const Json::Value &json);

    // getters
    OPT_GETTER(parentId, ParentId)
    GETTER(deptIds, DeptIds)
};
