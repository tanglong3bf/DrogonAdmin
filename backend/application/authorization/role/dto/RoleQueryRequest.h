#pragma once

#include "common/util/Utilities.hpp"
#include <drogon/Attribute.h>
#include <drogon/utils/Utilities.h>
#include <optional>
#include <string>
#include <cstdint>

struct RoleQueryRequest
{
    RoleQueryRequest(const std::string &name,
                     const std::string &deptId,
                     const std::string &page,
                     const std::string &pageSize,
                     const drogon::AttributesPtr &attr);

    GETTER(name)
    GETTER(deptId)
    GETTER(page)
    GETTER(pageSize)

  private:
    static size_t parseInt(const std::string &strValue, size_t defaultVal)
    {
        if (strValue.empty())
            return defaultVal;
        size_t val = drogon::utils::fromString<size_t>(strValue);
        return val == 0 ? defaultVal : val;
    }

  private:
    std::optional<std::string> name_;     ///< 角色名称
    std::optional<std::int32_t> deptId_;  ///< 部门
    std::size_t page_;                    ///< 页码
    std::size_t pageSize_;                ///< 每页数量
};
