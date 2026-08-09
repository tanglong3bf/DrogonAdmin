#pragma once

#include "common/util/Utilities.hpp"
#include <jsoncpp/json/value.h>
#include <optional>

/**
 * @brief 更新部门参数
 */
struct ModuleSortRequest
{
    ModuleSortRequest() = default;
    ModuleSortRequest(const Json::Value &json);

    GETTER(parentId)
    GETTER(moduleIds)

  private:
    std::optional<std::int32_t> parentId_;  ///< 父模块id
    std::vector<std::int32_t> moduleIds_;   ///< 模块id
};
