#pragma once

#include "common/util/Utilities.hpp"
#include <jsoncpp/json/value.h>
#include <string>

/**
 * @brief 更新部门参数
 */
class DeptUpdateRequest
{
    std::string name_;  ///< 部门名称

  public:
    void setByJson(const Json::Value &json);

    // getters
    GETTER(name, Name)
};
