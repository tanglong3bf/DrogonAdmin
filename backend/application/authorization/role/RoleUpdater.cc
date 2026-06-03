#include "RoleUpdater.h"

#include <unordered_set>
#include "common/util/Utilities.hpp"
#include "common/exception/BusinessException.h"
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
    ENTITY_SET(role, description, isUpdated = true);
    ENTITY_SET(role, quotaType, isUpdated = true; isQuotaUpdated = true);
    ENTITY_SET(role, userQuota, isUpdated = true; isQuotaUpdated = true);
    ENTITY_SET(role, relationType, isUpdated = true; isQuotaUpdated = true);

    if (request.deptIds())
    {
        vector<int> deptIds = *request.deptIds();
        sort(deptIds.begin(), deptIds.end());

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
    }

    if (role.relationType == RelationType::All && role.roleDepts.size() != 0)
    {
        for (auto &dept : role.roleDepts)
        {
            const_cast<RoleDept &>(dept).markDeleted();
        }
        isUpdated = true;
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
    else
    {
        throw BusinessException("角色数据无更新");
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
