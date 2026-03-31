#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "common/util/Utilities.hpp"

class GetRoleListRequest
{
    std::optional<std::string> name_;
    std::optional<std::int32_t> deptId_;
    std::int32_t page_;
    std::int32_t pageSize_;

  public:
    OPT_SETTER(name, Name)
    OPT_SETTER(deptId, DeptId)
    SETTER(page, Page)
    SETTER(pageSize, PageSize)

    OPT_GETTER(name, Name)
    OPT_GETTER(deptId, DeptId)
    GETTER(page, Page)
    GETTER(pageSize, PageSize)
};
