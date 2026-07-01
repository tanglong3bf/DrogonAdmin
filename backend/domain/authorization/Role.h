#pragma once

#include "RoleDept.h"
#include "RelationType.h"
#include "QuotaType.h"
#include "domain/models/SysRole.h"
#include "common/framework/domain/AuditableEntity.h"
#include "common/framework/domain/ChangeableEntity.h"
#include "common/util/ParamGetter.hpp"
#include "domain/models/SysRoleDept.h"

class Role : public AuditableEntity, public ChangeableEntity
{
    using SysRole = drogon_model::drogon_admin_db::SysRole;
    using SysRoleDept = drogon_model::drogon_admin_db::SysRoleDept;

  public:
    /**
     * @brief 准备必备参数的构造
     */
    Role(std::string_view name,
         std::string_view code,
         const QuotaType quotaType,
         const RelationType relationType);

    /**
     * @brief 准备必备参数以及创建者id的构造
     */
    Role(std::string_view name,
         std::string_view code,
         const QuotaType quotaType,
         const RelationType relationType,
         const std::int32_t createdBy);

    explicit Role(const SysRole &sysRole);
    explicit operator SysRole() const;

    void constructOptionalFields(
        std::optional<std::string> description,
        std::optional<std::int32_t> userQuota,
        const std::optional<std::vector<std::int32_t>> &deptIds,
        const std::int32_t createdBy);

    void updateBasicInfo(
        std::optional<std::string_view> name,
        std::optional<std::string_view> code,
        const drogon_admin::util::NullableValue<std::string_view> &description,
        std::int32_t updatedBy = -1);

    bool updateQuota(
        std::optional<QuotaType> quotaType,
        const drogon_admin::util::NullableValue<std::int32_t> userQuota,
        const std::optional<RelationType> relationType,
        std::int32_t updatedBy);

    bool updateRoleDepts(const std::vector<int32_t> &newDeptIds,
                         const int32_t updatedBy);

    void remove(std::int32_t deletedBy);

    GETTER(roleId)
    GETTER(name)
    GETTER(code)
    GETTER(description)
    GETTER(quotaType)
    GETTER(userQuota)
    GETTER(relationType)
    GETTER(roleDepts)

    /**
     * @brief 追加部门（仅新增，不删除已有角色）
     * @param newDeptIds 待新增部门ID列表
     * @param createdBy 操作人ID，用于填充新建部门审计字段
     */
    void appendDepts(const std::vector<int32_t> &newDeptIds,
                     const int32_t createdBy);

    /**
     * @brief 差量对齐更新部门：保留交集、删除不在新列表的旧部门、新增缺少部门
     * @param newRoleIds 最终需要持有的部门ID集合
     * @param updatedBy 本次更新操作人
     */
    void replaceDepts(const std::vector<int32_t> &roleIds,
                      const int32_t updatedBy);

    // 仓储重建聚合时调用
    void restoreDepts(const std::vector<SysRoleDept> &sysRoleDepts);

  private:
    // 新增时没有id
    std::optional<std::int32_t> roleId_;      ///< 角色id
    std::string name_;                        ///< 角色名称
    std::string code_;                        ///< 角色代码
    std::optional<std::string> description_;  ///< 描述
    QuotaType quotaType_;                     ///< 用户数量限制类型
    std::optional<std::int32_t> userQuota_;   ///< 具体限制数量
    RelationType relationType_;               ///< 部门关联关系
    std::vector<RoleDept> roleDepts_;         ///< 关联部门
};
