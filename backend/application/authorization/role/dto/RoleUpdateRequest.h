#pragma once

#include "common/util/ParamGetter.hpp"
#include "domain/authorization/DeptScopeType.h"
#include "domain/authorization/QuotaType.h"
#include "common/util/Utilities.hpp"
#include <jsoncpp/json/value.h>

/**
 * @brief 角色更新请求
 */
class RoleUpdateRequest
{
    std::optional<std::string> name_;                             ///< 角色名称
    std::optional<std::string> code_;                             ///< 角色编码
    drogon_admin::util::NullableValue<std::string> description_;  ///< 角色描述
    std::optional<QuotaType> quotaType_;                          ///< 限制类型
    drogon_admin::util::NullableValue<int32_t> userQuota_;        ///< 限制数量
    std::optional<RelationType> relationType_;                    ///< 关联类型
    std::optional<std::vector<int32_t>> deptIds_;                 ///< 关联部门

  public:
    void setByJson(const Json::Value &json);

    OPT_GETTER(name, Name)
    OPT_GETTER(code, Code)
    OPT_GETTER(description, Description)
    OPT_GETTER(quotaType, QuotaType)
    OPT_GETTER(userQuota, UserQuota)
    OPT_GETTER(relationType, RelationType)
    OPT_GETTER(deptIds, DeptIds)
};
