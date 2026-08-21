#pragma once

#include "common/util/Utilities.hpp"
#include <jsoncpp/json/value.h>
#include <optional>

struct ModuleSortItem
{
    std::int32_t moduleId;
    std::int32_t sortNum;
    std::int32_t version;
};

/**
 * @brief 更新部门参数
 */
struct ModuleSortRequest
{
    ModuleSortRequest() = default;
    ModuleSortRequest(const Json::Value &json);

    GETTER(parentId)
    GETTER(modules)

    std::vector<std::int32_t> moduleIds() const;

  private:
    std::optional<std::int32_t> parentId_;  ///< 父模块id
    std::vector<ModuleSortItem> modules_;   ///< 模块排序项
};
