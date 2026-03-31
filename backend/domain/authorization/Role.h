#pragma once

#include "common/framework/domain/AuditableEntity.h"
#include "common/framework/domain/ChangeableEntity.h"
#include "common/util/Utilities.hpp"
#include "RoleDept.h"
#include "DeptScopeType.h"
#include "QuotaType.h"
#include "domain/models/SysRole.h"


class Role : public AuditableEntity, public ChangeableEntity
{
    using SysRole = drogon_model::drogon_admin_db::SysRole;

    // 新增时没有id
    std::optional<std::int32_t> roleId_;
    std::string name_;
    std::string code_;
    std::optional<std::string> description_;
    QuotaType quotaType_;
    std::optional<std::int32_t> userQuota_;
    RelationType relationType_;
    std::vector<RoleDept> depts_;

  public:
    /**
     * @brief 准备必备参数的构造
     */
    Role(const std::string &name,
         const std::string &code,
         const QuotaType quotaType,
         const RelationType relationType_);

    /**
     * @brief 准备必备参数以及创建者id的构造
     */
    Role(const std::string &name,
         const std::string &code,
         const QuotaType quotaType,
         const RelationType relationType_,
         const std::int32_t createdBy);

    explicit Role(const SysRole &sysRole);
    explicit operator SysRole() const;

    OPT_GETTER(roleId, RoleId)
    GETTER(name, Name)
    GETTER(code, Code)
    OPT_GETTER(description, Description)
    GETTER(quotaType, QuotaType)
    OPT_GETTER(userQuota, UserQuota)
    GETTER(relationType, RelationType)
    GETTER(depts, Depts)

    OPT_SETTER(roleId, RoleId)
    OPT_SETTER(description, Description)
    OPT_SETTER(userQuota, UserQuota)
    SETTER(depts, Depts)

    void addRoleDept(const RoleDept &dept);
    void setRoleDepts(const std::vector<RoleDept> &depts);
};
