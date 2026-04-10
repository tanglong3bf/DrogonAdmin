#include "UserUpdater.h"
#include <unordered_set>
#include "common/exception/BusinessException.h"

using namespace std;
using namespace drogon;

Task<> UserUpdater::updateUser(User &user,
                               const UserUpdateRequest &request,
                               const int32_t updatedBy) const
{
    bool isUpdated = false;
    SET_ENTITY_FIELD_IF_CHANGED(user, Nickname, isUpdated = true);
    SET_ENTITY_FIELD_IF_CHANGED(user, Sex, isUpdated = true);
    SET_ENTITY_FIELD_IF_CHANGED(user, DeptId, isUpdated = true);
    SET_ENTITY_FIELD_IF_CHANGED(user, PhoneNumber, isUpdated = true);
    SET_ENTITY_FIELD_IF_CHANGED(user, Email, isUpdated = true);
    SET_ENTITY_FIELD_IF_CHANGED(user, Status, isUpdated = true);

    if (request.getRoleIds())
    {
        vector<int> roleIds = *request.getRoleIds();
        sort(roleIds.begin(), roleIds.end());

        updateUserRoles(const_cast<vector<UserRole> &>(user.getUserRoles()),
                        roleIds,
                        *user.getUserId(),
                        updatedBy);

        isUpdated = true;
    }

    if (isUpdated)
    {
        user.setUpdatedBy(updatedBy);
    }
    else
    {
        throw BusinessException("角色数据无更新");
    }
    co_return;
}

void UserUpdater::updateUserRoles(vector<UserRole> &userRoles,
                                  const vector<int32_t> &newRoleIds,
                                  const int32_t userId,
                                  const int32_t updatedBy) const
{
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
