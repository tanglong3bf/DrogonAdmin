#pragma once

#include <jsoncpp/json/value.h>
#include "common/util/Utilities.hpp"
#include "domain/authorization/DeptScopeType.h"
#include "domain/authorization/QuotaType.h"

/**
 * @brief 角色创建请求
 *
 * @see Role
 */
class RoleCreateRequest
{
    std::string name_;                        ///< 角色名称
    std::string code_;                        ///< 角色代码
    std::optional<std::string> description_;  ///< 角色描述
    QuotaType quotaType_;                     ///< 限制类型
    std::optional<int> userQuota_;            ///< 限制数量
    RelationType relationType_;               ///< 关联类型
    std::vector<int> deptIds_;                ///< 关联的部门ID列表

  public:
    void setByJson(const Json::Value &json);

    GETTER(name, Name)
    GETTER(code, Code)
    OPT_GETTER(description, Description)
    GETTER(quotaType, QuotaType)
    OPT_GETTER(userQuota, UserQuota)
    GETTER(relationType, RelationType)
    GETTER(deptIds, DeptIds)
};
