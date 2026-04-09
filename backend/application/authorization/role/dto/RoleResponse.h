#pragma once

#include <drogon/orm/Result.h>
#include <jsoncpp/json/value.h>
#include "RoleDeptResponse.h"
#include "common/util/Utilities.hpp"
#include "domain/authorization/DeptScopeType.h"
#include "domain/authorization/Role.h"

/**
 * @brief 角色响应
 */
class RoleResponse
{
    std::int32_t roleId_;                     ///< 角色ID
    std::string name_;                        ///< 角色名称
    std::string code_;                        ///< 角色代码
    std::optional<std::string> description_;  ///< 角色描述
    QuotaType quotaType_;                     ///< 限制类型
    std::optional<std::int32_t> userQuota_;   ///< 限制数量
    RelationType relationType_;               ///< 关联类型
    std::vector<RoleDeptResponse> depts;      ///< 关联部门

  public:
    RoleResponse(const Role &role);

  public:
    Json::Value toJson() const;

    GETTER(roleId, RoleId);

    void addRoleDept(const RoleDeptResponse &roleDept);
};
