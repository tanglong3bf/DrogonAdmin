#include "UserUpdater.h"

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
    // 更新 phone_number
    if (request.getPhoneNumber() &&
        user.getPhoneNumber() != *request.getPhoneNumber())
    {
        user.setPhoneNumber(*request.getPhoneNumber());
        isUpdated = true;
    }
    // 置空 phone_number
    else if (request.getPhoneNumber().isNull() && user.getPhoneNumber())
    {
        user.setPhoneNumberToNullOpt();
        isUpdated = true;
    }
    // 更新 email
    if (request.getEmail() && user.getEmail() != *request.getEmail())
    {
        user.setEmail(*request.getEmail());
        isUpdated = true;
    }
    // 置空 email
    else if (request.getEmail().isNull() && user.getEmail())
    {
        user.setEmailToNullOpt();
        isUpdated = true;
    }
    ENTITY_SET(user, Status, isUpdated = true);

    // 用户现有的角色id列表
    const auto oldRoleIds =
        user.getUserRoles() |
        views::transform([](const UserRole &ur) { return ur.getRoleId(); }) |
        ranges::to<vector>();
    vector<int> roleIds = {};
    if (request.getRoleIds())
    {
        roleIds = *request.getRoleIds();
    }
    // 需要新增的角色id列表
    const auto needInsertRoleIds =
        roleIds | views::filter([&](int32_t id) {
            return find(oldRoleIds.begin(), oldRoleIds.end(), id) ==
                   oldRoleIds.end();
        }) |
        ranges::to<vector>();
    // 需要删除的角色id列表
    const auto needDeleteRoleIds =
        oldRoleIds | views::filter([&](int32_t id) {
            return find(roleIds.begin(), roleIds.end(), id) == roleIds.end();
        }) |
        ranges::to<vector>();

    if (deptIdUpdated || needInsertRoleIds.size() > 0 ||
        needDeleteRoleIds.size() > 0)
    {
        sort(roleIds.begin(), roleIds.end());
        // 检查是否可以为指定部门分配这些角色
        if (deptIdUpdated)
        {
            co_await deptVerifier_->verifyRoleAssignmentAllowed(
                user.getDeptId(), roleIds);
        }
        else if (needInsertRoleIds.size() > 0)
        {
            co_await deptVerifier_->verifyRoleAssignmentAllowed(
                user.getDeptId(), needInsertRoleIds);
        }

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
