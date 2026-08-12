#pragma once

#include "RoleDeptResponse.h"
#include "domain/iam/role/RelationType.h"
#include "domain/iam/role/Role.h"
#include "common/util/Utilities.hpp"
#include <drogon/orm/Result.h>
#include <jsoncpp/json/value.h>

/**
 * @brief 角色响应
 */
struct RoleResponse
{
  public:
    RoleResponse(const Role &role);

    Json::Value toJson() const;

    GETTER(roleId)

    void addRoleDept(const RoleDeptResponse &roleDept);

  private:
    const std::int32_t roleId_;                     ///< 角色ID
    const std::string name_;                        ///< 角色名称
    const std::string code_;                        ///< 角色代码
    const std::optional<std::string> description_;  ///< 角色描述
    const QuotaType quotaType_;                     ///< 限制类型
    const std::optional<std::int32_t> userQuota_;   ///< 限制数量
    const RelationType relationType_;               ///< 关联类型
    std::vector<RoleDeptResponse> depts_;           ///< 关联部门
};
