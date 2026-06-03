#pragma once

#include "domain/authorization/DeptScopeType.h"
#include "domain/authorization/QuotaType.h"
#include "common/util/Utilities.hpp"
#include <jsoncpp/json/value.h>

/**
 * @brief 角色更新请求
 */
struct RoleUpdateRequest
{
    RoleUpdateRequest() = default;
    RoleUpdateRequest(const Json::Value &json);

    GETTER(name)
    GETTER(code)
    GETTER(description)
    GETTER(quotaType)
    GETTER(userQuota)
    GETTER(relationType)
    GETTER(deptIds)

  private:
    std::optional<std::string> name_;              ///< 角色名称
    std::optional<std::string> code_;              ///< 角色编码
    std::optional<std::string> description_;       ///< 角色描述
    std::optional<QuotaType> quotaType_;           ///< 限制类型
    std::optional<int32_t> userQuota_;             ///< 限制数量
    std::optional<RelationType> relationType_;     ///< 关联类型
    std::optional<std::vector<int32_t>> deptIds_;  ///< 关联部门
};
