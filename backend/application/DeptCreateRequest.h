#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <jsoncpp/json/value.h>
#include "common/util/Utilities.hpp"

/**
 * @brief 创建部门参数
 */
class DeptCreateRequest
{
    std::string name_;                       ///< 部门名称
    std::optional<std::uint32_t> parentId_;  ///< 父部门ID

  public:
    void setByJson(const Json::Value &json);

    // getters
    GETTER(name, Name)
    OPT_GETTER(parentId, ParentId)
};
