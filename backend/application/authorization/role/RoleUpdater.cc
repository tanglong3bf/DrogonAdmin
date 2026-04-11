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
    bool isUpdated = false;
    ENTITY_SET(role, Name, isUpdated = true);
    ENTITY_SET(role, Code, isUpdated = true);
    ENTITY_SET(role, Description, isUpdated = true);
    ENTITY_SET(role, QuotaType, isUpdated = true);
    ENTITY_SET(role, UserQuota, isUpdated = true);
    ENTITY_SET(role, RelationType, isUpdated = true);

    if (request.getDeptIds())
    {
        vector<int> deptIds = *request.getDeptIds();
        sort(deptIds.begin(), deptIds.end());

        updateRoleDepts(const_cast<vector<RoleDept> &>(role.getRoleDepts()),
                        deptIds,
                        *role.getRoleId(),
                        updatedBy);

        isUpdated = true;
    }

    if (role.getQuotaType() == QuotaType::Unlimited)
    {
        if (role.getUserQuota() != nullopt)
        {
            role.setUserQuotaToNullOpt();
            isUpdated = true;
        }
    }
    if (role.getRelationType() == RelationType::All)
    {
        if (role.getRoleDepts().size() != 0)
        {
            for (auto &dept : role.getRoleDepts())
            {
                const_cast<RoleDept &>(dept).toDelete();
            }
            isUpdated = true;
        }
    }

    if (isUpdated)
    {
        // TODO: 黑白名单和数量限制
        LOG_WARN << "TODO: 用户管理完成后，检查用户表数据";
        role.setUpdatedBy(updatedBy);
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
