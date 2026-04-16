#include "UserUpdater.h"

#include "common/exception/BusinessException.h"
#include <unordered_set>

using namespace std;
using namespace drogon;

Task<> UserUpdater::updateUser(User &user,
                               const UserUpdateRequest &request,
                               const int32_t updatedBy) const
{
    LOG_TRACE << "更新用户，userId=" << *user.getUserId()
              << ", updatedBy=" << updatedBy;
    bool isUpdated = false;
    bool deptIdUpdated = false;
    ENTITY_SET(user, Nickname, isUpdated = true);
    ENTITY_SET(user, Sex, isUpdated = true);
    ENTITY_SET(user, DeptId, isUpdated = true; deptIdUpdated = true);
    ENTITY_SET(user, PhoneNumber, isUpdated = true);
    ENTITY_SET(user, Email, isUpdated = true);
    ENTITY_SET(user, Status, isUpdated = true);

    if (request.getRoleIds() || deptIdUpdated)
    {
        vector<int> roleIds = *request.getRoleIds();
        sort(roleIds.begin(), roleIds.end());
        // 检查是否可以为指定部门分配这些角色
        co_await deptVerifier_->verifyRoleAssignmentAllowed(user.getDeptId(),
                                                            roleIds);

        updateUserRoles(const_cast<vector<UserRole> &>(user.getUserRoles()),
                        roleIds,
                        *user.getUserId(),
                        updatedBy);

        isUpdated = true;
    }

    if (isUpdated)
    {
        user.setUpdatedBy(updatedBy);
        user.toUpdate();
    }
    else
    {
        throw BusinessException("用户数据无更新");
    }
    co_return;
}

void UserUpdater::updateUserRoles(vector<UserRole> &userRoles,
                                  const vector<int32_t> &newRoleIds,
                                  const int32_t userId,
                                  const int32_t updatedBy) const
{
    LOG_TRACE << "更新用户角色，userId=" << userId
              << ", updatedBy=" << updatedBy;
    LOG_TRACE << "原角色列表: " << userRoles.size() << "个角色";
    LOG_TRACE << "新角色列表: " << newRoleIds.size() << "个角色";
    unordered_set<int32_t> newRoleSet(newRoleIds.begin(), newRoleIds.end());

    for (auto &ur : userRoles)
    {
        if (newRoleSet.find(ur.getRoleId()) == newRoleSet.end())
        {
            ur.toDelete();
        }
    }

    unordered_set<int32_t> existingRoleIds;
    for (const auto &rd : userRoles)
    {
        existingRoleIds.insert(rd.getRoleId());
    }

    for (int32_t roleId : newRoleIds)
    {
        if (existingRoleIds.find(roleId) == existingRoleIds.end())
        {
            UserRole newUR(roleId, updatedBy);
            newUR.setUserId(userId);
            newUR.toNew();
            userRoles.push_back(std::move(newUR));
        }
    }
}
