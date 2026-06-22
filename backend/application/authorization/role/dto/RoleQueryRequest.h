#pragma once

#include "common/util/Utilities.hpp"
#include <drogon/Attribute.h>
#include <optional>
#include <string>
#include <cstdint>

class RoleQueryRequest
{
    std::optional<std::string> name_;     ///< 角色名称
    std::optional<std::int32_t> deptId_;  ///< 部门
    std::size_t page_;                    ///< 页码
    std::size_t pageSize_;                ///< 每页数量

  public:
    RoleQueryRequest(const std::string &name,
                     const std::string &deptId,
                     const std::string &page,
                     const std::string &pageSize,
                     const drogon::AttributesPtr &attr);

    OPT_SETTER(name, Name)
    OPT_SETTER(deptId, DeptId)
    SETTER(page, Page)
    SETTER(pageSize, PageSize)

    OPT_GETTER(name, Name)
    OPT_GETTER(deptId, DeptId)
    GETTER(page, Page)
    GETTER(pageSize, PageSize)
};
