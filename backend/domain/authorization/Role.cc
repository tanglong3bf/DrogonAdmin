#include "Role.h"

#include "common/util/rangesUtils.hpp"
#include <unordered_set>
#include <ranges>

using namespace std;
using namespace trantor;
using namespace drogon_admin;
using namespace drogon_admin::util;
using namespace drogon_model::drogon_admin_db;

Role::Role(string_view name,
           string_view code,
           const QuotaType quotaType,
           const RelationType relationType)
    : name_{name},
      code_{code},
      quotaType_{quotaType},
      relationType_{relationType}
{
}

Role::Role(string_view name,
           string_view code,
           const QuotaType quotaType,
           const RelationType relationType,
           const int32_t createdBy)
    : name_{name},
      code_{code},
      quotaType_{quotaType},
      relationType_{relationType},
      AuditableEntity{createdBy, Date::now(), createdBy, Date::now()}
{
}

Role::Role(const SysRole &model)
    : INIT(roleId_, RoleId),
      INIT(name_, Name),
      INIT(code_, Code),
      OPT_INIT(description_, Description),
      ENUM_INIT(QuotaType, quotaType_, QuotaType),
      OPT_INIT(userQuota_, UserQuota),
      ENUM_INIT(RelationType, relationType_, RelationType),
      AuditableEntity(AUDITABLE_INIT_BY_MODEL)
{
}

Role::operator SysRole() const
{
    SysRole model;
    if (roleId_)
    {
        model.setRoleId(*roleId_);
    }
    model.setName(name_);
    model.setCode(code_);
    if (description_)
    {
        model.setDescription(*description_);
    }
    else
    {
        model.setDescriptionToNull();
    }
    model.setQuotaType(static_cast<int16_t>(quotaType_));
    if (userQuota_)
    {
        model.setUserQuota(*userQuota_);
    }
    else
    {
        model.setUserQuotaToNull();
    }
    model.setRelationType(static_cast<int16_t>(relationType_));
    model.setCreatedBy(*createdBy_);
    model.setCreatedTime(*createdTime_);
    model.setUpdatedBy(*updatedBy_);
    model.setUpdatedTime(*updatedTime_);
    if (deletedBy_)
    {
        model.setDeletedBy(*deletedBy_);
    }
    if (deletedTime_)
    {
        model.setDeletedTime(*deletedTime_);
    }
    return model;
}

void Role::constructOptionalFields(optional<string> description,
                                   optional<int32_t> userQuota,
                                   const optional<vector<int32_t>> &deptIds,
                                   const int32_t createdBy)
{
    if (const auto desc = description)
    {
        description_ = *desc;
    }
    if (quotaType_ != QuotaType::Unlimited)
    {
        userQuota_ = *userQuota;
    }
    if (relationType_ != RelationType::All)
    {
        if (!deptIds || deptIds->size() == 0)
        {
            throw std::runtime_error("请选择关联部门");
        }
        appendDepts(*deptIds, createdBy);
    }
}

void Role::updateBasicInfo(optional<string_view> name,
                           optional<string_view> code,
                           const NullableValue<string_view> &description,
                           int32_t updatedBy)
{
    bool isUpdated = false;
    if (name && name_ != *name)
    {
        name_ = *name;
        isUpdated = true;
    }
    if (code && code_ != *code)
    {
        code_ = *code;
        isUpdated = true;
    }
    if (description && description_ != *description)
    {
        description_ = *description;
        isUpdated = true;
    }
    else if (description.isNull() && description_ != std::nullopt)
    {
        description_ = std::nullopt;
        isUpdated = true;
    }
    if (updatedBy != -1)
    {
        updatedBy_ = updatedBy;
        isUpdated = true;
    }
    if (isUpdated)
    {
        markUpdatedBy(updatedBy);
        markUpdated();
    }
}

bool Role::updateQuota(optional<QuotaType> quotaType,
                       const NullableValue<int32_t> userQuota,
                       const optional<RelationType> relationType,
                       int32_t updatedBy)
{
    bool isUpdated = false;
    if (quotaType && quotaType_ != quotaType)
    {
        quotaType_ = *quotaType;
        isUpdated = true;
    }
    // 更新为新值
    if (userQuota && userQuota_ != *userQuota)
    {
        userQuota_ = *userQuota;
        isUpdated = true;
    }
    // 更新为空
    else if (userQuota.isNull() && userQuota_ != nullopt)
    {
        userQuota_ = nullopt;
        isUpdated = true;
    }
    if (relationType && relationType_ != *relationType)
    {
        relationType_ = *relationType;
        isUpdated = true;
    }
    if (quotaType_ == QuotaType::Unlimited && userQuota_ != nullopt)
    {
        userQuota_ = nullopt;
        isUpdated = true;
    }
    if (relationType_ == RelationType::All && roleDepts_.size() != 0)
    {
        for (auto &dept : roleDepts_)
        {
            const_cast<RoleDept &>(dept).markDeleted();
        }
        isUpdated = true;
    }
    if (quotaType_ != QuotaType::Unlimited && userQuota_ == nullopt)
    {
        throw BusinessException(
            "当quota_type不为unlimited时，user_quota不能为空");
    }
    if (isUpdated)
    {
        markUpdatedBy(updatedBy);
        markUpdated();
    }
    return isUpdated;
}

bool Role::updateRoleDepts(const vector<int32_t> &newDeptIds,
                           const int32_t updatedBy)
{
    bool isUpdated = false;
    unordered_set<int32_t> newDeptSet(newDeptIds.begin(), newDeptIds.end());

    for (auto &rd : roleDepts_)
    {
        if (newDeptSet.find(rd.deptId()) == newDeptSet.end())
        {
            rd.markDeleted();
            isUpdated = true;
        }
    }

    unordered_set<int32_t> existingDeptIds;
    for (const auto &rd : roleDepts_)
    {
        existingDeptIds.insert(rd.deptId());
    }

    for (int32_t deptId : newDeptSet)
    {
        if (existingDeptIds.find(deptId) == existingDeptIds.end())
        {
            RoleDept newRD(deptId, updatedBy);
            newRD.roleId_ = roleId_;
            newRD.markNew();
            roleDepts_.push_back(std::move(newRD));
            isUpdated = true;
        }
    }
    return isUpdated;
}

void Role::remove(int32_t deletedBy)
{
    markDeletedBy(deletedBy);
    markDeleted();
    for (auto &roleDept : roleDepts_)
    {
        roleDept.markDeleted();
    }
}

void Role::appendDepts(const std::vector<int32_t> &newDeptIds,
                       const int32_t createdBy)
{
    const auto newDepts =
        newDeptIds | views::transform([createdBy, this](const int32_t roleId) {
            RoleDept rd{roleId, createdBy};
            rd.roleId_ = roleId_;
            rd.markNew();
            return rd;
        }) |
        ranges_utils::to<vector>();

    roleDepts_.insert(roleDepts_.end(), newDepts.begin(), newDepts.end());
}

void Role::replaceDepts(const std::vector<int32_t> &deptIds,
                        const int32_t updatedBy)
{
    unordered_set<int32_t> newDeptSet(deptIds.begin(), deptIds.end());

    // 标记删除
    for (auto &rd : roleDepts_)
    {
        if (newDeptSet.find(rd.deptId()) == newDeptSet.end())
        {
            rd.markDeleted();
        }
    }

    // 准备已存在的角色id
    unordered_set<int32_t> existingDeptIds;
    for (const auto &rd : roleDepts_)
    {
        existingDeptIds.insert(rd.deptId());
    }

    for (int32_t roleId : deptIds)
    {
        // 没有在已拥有的部门id列表寻找到
        if (existingDeptIds.find(roleId) == existingDeptIds.end())
        {
            // 新增
            RoleDept newUR(roleId, updatedBy);
            newUR.roleId_ = roleId_;
            newUR.markNew();
            roleDepts_.push_back(std::move(newUR));
        }
    }
}

void Role::restoreDepts(const std::vector<SysRoleDept> &sysRoleDepts)
{
    roleDepts_ =
        sysRoleDepts |
        views::transform([](const SysRoleDept &rd) { return RoleDept{rd}; }) |
        ranges_utils::to<vector>();
}
