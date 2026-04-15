#include "UserUpdater.h"

#include "common/exception/BusinessException.h"
#include <unordered_set>

using namespace std;
using namespace drogon;

Task<> UserUpdater::updateUser(User &user,
                               const UserUpdateRequest &request,
                               const int32_t updatedBy) const
{
    bool isUpdated = false;
    ENTITY_SET(user, Nickname, isUpdated = true);
    ENTITY_SET(user, Sex, isUpdated = true);
    ENTITY_SET(user, DeptId, isUpdated = true);
    ENTITY_SET(user, PhoneNumber, isUpdated = true);
    ENTITY_SET(user, Email, isUpdated = true);
    ENTITY_SET(user, Status, isUpdated = true);

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
        throw BusinessException("用户数据无更新");
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
