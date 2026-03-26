#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <drogon/HttpRequest.h>
#include <jsoncpp/json/value.h>
#include "common/util/Utilities.hpp"

class GetRoleListRequest
{
    std::optional<std::string> name_;     ///< 角色名称，模糊查询
    std::optional<std::int32_t> deptId_;  ///< 部门id，查询该部门可用的角色
    std::int32_t page_{1};                ///< 页码，默认为1
    std::int32_t pageSize_{10};           ///< 每页数量，默认为10

  public:
    /**
     * @brief 从请求体获取请求参数
     */
    void setByReq(const drogon::HttpRequest &req);

    // getters
    OPT_GETTER(name, Name)
    OPT_GETTER(deptId, DeptId)
    GETTER(page, Page)
    GETTER(pageSize, PageSize)

    // setters
    SETTER(page, Page)
};
