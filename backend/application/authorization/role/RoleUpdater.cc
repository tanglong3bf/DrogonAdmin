#include "RoleUpdater.h"

#include <unordered_set>
#include "common/util/Utilities.hpp"
#include "domain/authorization/RoleDept.h"

using namespace std;
using namespace drogon;

Task<> RoleUpdater::updateRole(Role &role,
                               const RoleUpdateRequest &request,
                               const int32_t updatedBy) const
{
    const Role oldData = role;
    bool isUpdated = false;
    bool isQuotaUpdated = false;
    if (request.name() && role.name != *request.name())
    {
        co_await roleVerifier_->verifyRoleNameNotDuplicated(*request.name());
        role.name = *request.name();
        isUpdated = true;
    }
    if (request.code() && role.code != *request.code())
    {
        co_await roleVerifier_->verifyRoleCodeNotDuplicated(*request.code());
        role.code = *request.code();
        isUpdated = true;
    }
    // 更新为新值
    if (request.description() && role.description != *request.description())
    {
        role.description = *request.description();
        isUpdated = true;
    }
    // 更新为空
    else if (request.description().isNull() && role.description != nullopt)
    {
        role.description = nullopt;
        isUpdated = true;
    }
    ENTITY_SET(role, quotaType, isUpdated = true; isQuotaUpdated = true);
    // 更新为新值
    if (request.userQuota() && role.userQuota != *request.userQuota())
    {
        role.userQuota = *request.userQuota();
        isUpdated = true;
        isQuotaUpdated = true;
    }
    // 更新为空
    else if (request.userQuota().isNull() && role.userQuota != nullopt)
    {
        role.userQuota = nullopt;
        isUpdated = true;
        isQuotaUpdated = true;
    }
    ENTITY_SET(role, relationType, isUpdated = true; isQuotaUpdated = true);

    if (request.deptIds() && request.deptIds()->size() > 0)
    {
        vector<int> deptIds = *request.deptIds();
        sort(deptIds.begin(), deptIds.end());
        co_await deptVerifier_->verifyDeptIdsExist(deptIds);

        updateRoleDepts(const_cast<vector<RoleDept> &>(role.roleDepts),
                        deptIds,
                        *role.roleId,
                        updatedBy);

        isUpdated = true;
        isQuotaUpdated = true;
    }

    if (role.quotaType == QuotaType::Unlimited && role.userQuota != nullopt)
    {
        role.userQuota = nullopt;
        isUpdated = true;
        isQuotaUpdated = true;
    }

    if (role.quotaType != QuotaType::Unlimited && role.userQuota == nullopt)
    {
        throw BusinessException(
            "当quota_type不为unlimited时，user_quota不能为空");
    }

    if (role.relationType == RelationType::All && role.roleDepts.size() != 0)
    {
        for (auto &dept : role.roleDepts)
        {
            const_cast<RoleDept &>(dept).markDeleted();
        }
        isUpdated = true;
    }

    if (role.relationType != RelationType::All)
    {
        if (role.roleDepts.size() == 0)
        {
            throw BusinessException("当relation_type不为0时，dept_ids不能为空");
        }
        else
        {
            vector<int32_t> deptIds;
            if (request.deptIds())
            {
                deptIds = *request.deptIds();
            }
            else
            {
                for (const auto &rd : role.roleDepts)
                {
                    deptIds.push_back(rd.deptId());
                }
            }
            if (role.relationType == RelationType::Whitelist)
            {
                co_await userVerifier_->verifyUsersWithRoleBelongToDepts(
                    *role.roleId, deptIds);
            }
            else
            {
                co_await userVerifier_->verifyUsersWithRoleNotBelongToDepts(
                    *role.roleId, deptIds);
            }
        }
    }

    if (isQuotaUpdated)
    {
        co_await roleVerifier_->checkQuota(role, oldData);
    }
    if (isUpdated)
    {
        role.markUpdatedBy(updatedBy);
        role.markUpdated();
    }
    co_return;
}

void RoleUpdater::updateRoleDepts(vector<RoleDept> &roleDepts,
                                  const vector<int32_t> &newDeptIds,
                                  const int32_t roleId,
                                  const int32_t updatedBy) const
{
    unordered_set<int32_t> newDeptSet(newDeptIds.begin(), newDeptIds.end());

    for (auto &rd : roleDepts)
    {
        if (newDeptSet.find(rd.deptId()) == newDeptSet.end())
        {
            rd.markDeleted();
        }
    }

    unordered_set<int32_t> existingDeptIds;
    for (const auto &rd : roleDepts)
    {
        existingDeptIds.insert(rd.deptId());
    }

    for (int32_t deptId : newDeptSet)
    {
        if (existingDeptIds.find(deptId) == existingDeptIds.end())
        {
            RoleDept newRD(deptId, updatedBy);
            newRD.roleId = roleId;
            newRD.markNew();
            roleDepts.push_back(std::move(newRD));
        }
    }
}
