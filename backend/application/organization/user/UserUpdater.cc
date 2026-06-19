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
    ENTITY_SET(user, Nickname, isUpdated = true);
    ENTITY_SET(user, Sex, isUpdated = true);
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

    const auto oldDeptId = user.getDeptId();

    // 部门id
    if (request.getDeptId() && user.getDeptId() != *request.getDeptId())
    {
        co_await deptVerifier_->verifyDepartmentExists(*request.getDeptId());
        user.setDeptId(*request.getDeptId());
        isUpdated = true;
    }
    const auto newDeptId = user.getDeptId();

    // 角色列表
    if (request.getRoleIds())
    {
        co_await roleVerifier_->verifyRolesExists(*request.getRoleIds());
        updateUserRoles(const_cast<vector<UserRole> &>(user.getUserRoles()),
                        *request.getRoleIds(),
                        *user.getUserId(),
                        updatedBy);
        isUpdated = true;
    }
    const auto allRoleIds =
        user.getUserRoles() | views::filter([](const auto &ur) {
            return ur.getChangingStatus() != ChangingStatus::DELETED;
        }) |
        views::transform([](const auto &ur) { return ur.getRoleId(); }) |
        ranges::to<vector<int32_t>>();
    const auto newRoleIds =
        user.getUserRoles() | views::filter([](const auto &ur) {
            return ur.getChangingStatus() == ChangingStatus::NEW;
        }) |
        views::transform([](const auto &ur) { return ur.getRoleId(); }) |
        ranges::to<vector<int32_t>>();
    co_await roleVerifier_->verifyDeptRolesAllowedForNewUser(oldDeptId,
                                                             newDeptId,
                                                             allRoleIds,
                                                             newRoleIds);

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
    unordered_set<int32_t> newRoleSet(newRoleIds.begin(), newRoleIds.end());

    // 标记删除
    for (auto &ur : userRoles)
    {
        if (newRoleSet.find(ur.getRoleId()) == newRoleSet.end())
        {
            ur.toDelete();
        }
    }

    // 准备已存在的角色id
    unordered_set<int32_t> existingRoleIds;
    for (const auto &rd : userRoles)
    {
        existingRoleIds.insert(rd.getRoleId());
    }

    for (int32_t roleId : newRoleIds)
    {
        // 没有在已拥有的角色id列表寻找到
        if (existingRoleIds.find(roleId) == existingRoleIds.end())
        {
            // 新增
            UserRole newUR(roleId, updatedBy);
            newUR.setUserId(userId);
            newUR.toNew();
            userRoles.push_back(std::move(newUR));
        }
    }
}
