#pragma once

#include "common/framework/domain/AuditableEntity.h"
#include "common/framework/domain/ChangeableEntity.h"
#include "common/util/Utilities.hpp"
#include "domain/models/SysRole.h"

// 用户数量限制类型
enum class QuotaType
{
    Unlimited = 0,    // 不限制
    TotalLimit = 1,   // 总数量限制
    PerDeptLimit = 2  // 每个部门用户数量限制
};

template <>
std::string toString(const QuotaType &quotaType);

class Role : public AuditableEntity, public ChangeableEntity
{
    std::optional<std::int32_t> roleId_;
    std::string name_;
    std::string code_;
    std::optional<std::string> description_;
    std::optional<std::int32_t> userQuota_;
    QuotaType quotaType_;

  public:
    Role(const drogon_model::drogon_admin_db::SysRole &sysRole);

    OPT_GETTER(roleId, RoleId)
    GETTER(name, Name)
    GETTER(code, Code)
    OPT_GETTER(description, Description)
    OPT_GETTER(userQuota, UserQuota)
    GETTER(quotaType, QuotaType)
};
