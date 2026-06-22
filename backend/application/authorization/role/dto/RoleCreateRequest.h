#pragma once

#include "domain/authorization/DeptScopeType.h"
#include "domain/authorization/QuotaType.h"
#include "common/util/Utilities.hpp"
#include <jsoncpp/json/value.h>
#include <optional>

/**
 * @brief 角色创建请求
 *
 * @see Role
 */
struct RoleCreateRequest
{
    RoleCreateRequest() = default;
    RoleCreateRequest(const Json::Value &json);

    std::string_view name() const
    {
        return name_;
    }

    std::string_view code() const
    {
        return code_;
    }

    GETTER(description)
    GETTER(quotaType)
    GETTER(userQuota)
    GETTER(relationType)
    GETTER(deptIds)

  private:
    std::string name_;                        ///< 角色名称
    std::string code_;                        ///< 角色代码
    std::optional<std::string> description_;  ///< 角色描述
    QuotaType quotaType_;                     ///< 限制类型
    std::optional<int32_t> userQuota_;        ///< 限制数量
    RelationType relationType_;               ///< 关联类型
    std::vector<std::int32_t> deptIds_;       ///< 关联部门
};
