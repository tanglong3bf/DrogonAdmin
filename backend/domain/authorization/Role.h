#pragma once

#include "common/framework/domain/AuditableEntity.h"
#include "common/framework/domain/ChangeableEntity.h"
#include "common/util/Utilities.hpp"
#include "domain/models/SysRole.h"
#include "RoleDept.h"
#include "DeptScopeType.h"
#include "QuotaType.h"

class Role : public AuditableEntity, public ChangeableEntity
{
    using SysRole = drogon_model::drogon_admin_db::SysRole;

    // 新增时没有id
    std::optional<std::int32_t> roleId_;      ///< 角色id
    std::string name_;                        ///< 角色名称
    std::string code_;                        ///< 角色代码
    std::optional<std::string> description_;  ///< 描述
    QuotaType quotaType_;                     ///< 用户数量限制类型
    std::optional<std::int32_t> userQuota_;   ///< 具体限制数量
    RelationType relationType_;               ///< 部门关联关系
    std::vector<RoleDept> roleDepts_;         ///< 关联部门

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
    GETTER(roleDepts, RoleDepts)

    OPT_SETTER(roleId, RoleId)
    SETTER(name, Name)
    SETTER(code, Code)
    OPT_SETTER(description, Description)
    SETTER(quotaType, QuotaType)
    OPT_SETTER(userQuota, UserQuota)
    SETTER(relationType, RelationType)
    SETTER(roleDepts, RoleDepts)

    void addRoleDept(const RoleDept &dept);
};
