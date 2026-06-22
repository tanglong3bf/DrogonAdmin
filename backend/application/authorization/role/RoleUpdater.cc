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
    if (request.getName() && role.getName() != *request.getName())
    {
        co_await roleVerifier_->verifyRoleNameNotDuplicated(*request.getName());
        role.setName(*request.getName());
        isUpdated = true;
    }
    if (request.getCode() && role.getCode() != *request.getCode())
    {
        co_await roleVerifier_->verifyRoleCodeNotDuplicated(*request.getCode());
        role.setCode(*request.getCode());
        isUpdated = true;
    }
    // 更新为新值
    if (request.getDescription() &&
        role.getDescription() != *request.getDescription())
    {
        role.setDescription(*request.getDescription());
        isUpdated = true;
    }
    // 更新为空
    else if (request.getDescription().isNull() &&
             role.getDescription() != nullopt)
    {
        role.setDescriptionToNullOpt();
        isUpdated = true;
    }
    ENTITY_SET(role, QuotaType, isUpdated = true; isQuotaUpdated = true);
    // 更新为新值
    if (request.getUserQuota() &&
        role.getUserQuota() != *request.getUserQuota())
    {
        role.setUserQuota(*request.getUserQuota());
        isUpdated = true;
        isQuotaUpdated = true;
    }
    // 更新为空
    else if (request.getUserQuota().isNull() && role.getUserQuota() != nullopt)
    {
        role.setUserQuotaToNullOpt();
        isUpdated = true;
        isQuotaUpdated = true;
    }
    ENTITY_SET(role, RelationType, isUpdated = true; isQuotaUpdated = true);

    if (request.getDeptIds())
    {
        vector<int> deptIds = *request.getDeptIds();
        sort(deptIds.begin(), deptIds.end());
        co_await deptVerifier_->verifyDeptIdsExist(deptIds);

        updateRoleDepts(const_cast<vector<RoleDept> &>(role.getRoleDepts()),
                        deptIds,
                        *role.getRoleId(),
                        updatedBy);

        isUpdated = true;
        isQuotaUpdated = true;
    }

    if (role.getQuotaType() == QuotaType::Unlimited &&
        role.getUserQuota() != nullopt)
    {
        role.setUserQuotaToNullOpt();
        isUpdated = true;
        isQuotaUpdated = true;
    }

    if (role.getQuotaType() != QuotaType::Unlimited &&
        role.getUserQuota() == nullopt)
    {
        throw BusinessException(
            "当quota_type不为unlimited时，user_quota不能为空");
    }

    if (role.getRelationType() == RelationType::All &&
        role.getRoleDepts().size() != 0)
    {
        for (auto &dept : role.getRoleDepts())
        {
            const_cast<RoleDept &>(dept).toDelete();
        }
        isUpdated = true;
    }

    if (role.getRelationType() != RelationType::All)
    {
        if (role.getRoleDepts().size() == 0)
        {
            throw BusinessException("当relation_type不为0时，dept_ids不能为空");
        }
        else
        {
            vector<int32_t> deptIds;
            if (request.getDeptIds())
            {
                deptIds = *request.getDeptIds();
            }
            else
            {
                for (const auto &rd : role.getRoleDepts())
                {
                    deptIds.push_back(rd.getDeptId());
                }
            }
            if (role.getRelationType() == RelationType::Whitelist)
            {
                co_await userVerifier_->verifyUsersWithRoleBelongToDepts(
                    *role.getRoleId(), deptIds);
            }
            else
            {
                co_await userVerifier_->verifyUsersWithRoleNotBelongToDepts(
                    *role.getRoleId(), deptIds);
            }
        }
    }

    if (isQuotaUpdated)
    {
        co_await roleVerifier_->checkQuota(role, oldData);
    }
    if (isUpdated)
    {
        role.setUpdatedBy(updatedBy);
        role.toUpdate();
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
        if (newDeptSet.find(rd.getDeptId()) == newDeptSet.end())
        {
            rd.toDelete();
        }
    }

    unordered_set<int32_t> existingDeptIds;
    for (const auto &rd : roleDepts)
    {
        existingDeptIds.insert(rd.getDeptId());
    }

    for (int32_t deptId : newDeptSet)
    {
        if (existingDeptIds.find(deptId) == existingDeptIds.end())
        {
            RoleDept newRD(deptId, updatedBy);
            newRD.setRoleId(roleId);
            newRD.toNew();
            roleDepts.push_back(std::move(newRD));
        }
    }
}
