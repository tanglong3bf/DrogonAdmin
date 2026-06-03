#pragma once

#include "common/framework/domain/AuditableEntity.h"
#include "common/framework/domain/ChangeableEntity.h"
#include "domain/models/SysRole.h"
#include "RoleDept.h"
#include "DeptScopeType.h"
#include "QuotaType.h"

class Role : public AuditableEntity, public ChangeableEntity
{
    using SysRole = drogon_model::drogon_admin_db::SysRole;

  public:
    /**
     * @brief 准备必备参数的构造
     */
    Role(const std::string &name,
         const std::string &code,
         const QuotaType quotaType,
         const RelationType relationType);

    /**
     * @brief 准备必备参数以及创建者id的构造
     */
    Role(const std::string &name,
         const std::string &code,
         const QuotaType quotaType,
         const RelationType relationType,
         const std::int32_t createdBy);

    explicit Role(const SysRole &sysRole);
    explicit operator SysRole() const;

    void setRoleDepts(const std::vector<RoleDept> &roleDepts);
    void addRoleDept(const RoleDept &dept);

  public:
    // 新增时没有id
    std::optional<std::int32_t> roleId;      ///< 角色id
    std::string name;                        ///< 角色名称
    std::string code;                        ///< 角色代码
    std::optional<std::string> description;  ///< 描述
    QuotaType quotaType;                     ///< 用户数量限制类型
    std::optional<std::int32_t> userQuota;   ///< 具体限制数量
    RelationType relationType;               ///< 部门关联关系
    std::vector<RoleDept> roleDepts;         ///< 关联部门
};
