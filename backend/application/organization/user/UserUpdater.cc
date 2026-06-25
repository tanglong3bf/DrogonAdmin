#include "UserUpdater.h"

#include "common/util/rangesUtils.hpp"
#include <unordered_set>
#include <ranges>

using namespace std;
using namespace drogon;
using namespace tl;

Task<> UserUpdater::updateUser(User &user,
                               const UserUpdateRequest &request,
                               const int32_t updatedBy) const
{
    LOG_TRACE << "更新用户，userId=" << *user.userId
              << ", updatedBy=" << updatedBy;
    bool isUpdated = false;
    ENTITY_SET(user, nickname, isUpdated = true);
    ENTITY_SET(user, sex, isUpdated = true);
    // 更新 phone_number
    if (request.phoneNumber() && user.phoneNumber != *request.phoneNumber())
    {
        user.phoneNumber = *request.phoneNumber();
        isUpdated = true;
    }
    // 置空 phone_number
    else if (request.phoneNumber().isNull() && user.phoneNumber)
    {
        user.phoneNumber = nullopt;
        isUpdated = true;
    }
    // 更新 email
    if (request.email() && user.email != *request.email())
    {
        user.email = *request.email();
        isUpdated = true;
    }
    // 置空 email
    else if (request.email().isNull() && user.email)
    {
        user.email = nullopt;
        isUpdated = true;
    }
    ENTITY_SET(user, status, isUpdated = true);

    const auto oldDeptId = user.deptId;

    // 部门id
    if (request.deptId() && user.deptId != *request.deptId())
    {
        co_await deptVerifier_->verifyDepartmentExists(*request.deptId());
        user.deptId = *request.deptId();
        isUpdated = true;
    }
    const auto newDeptId = user.deptId;

    // 角色列表
    if (request.roleIds())
    {
        co_await roleVerifier_->verifyRolesExists(*request.roleIds());
        updateUserRoles(const_cast<vector<UserRole> &>(user.userRoles),
                        *request.roleIds(),
                        *user.userId,
                        updatedBy);
        isUpdated = true;
    }
    const auto allRoleIds =
        user.userRoles | views::filter([](const auto &ur) {
            return ur.changingStatus() != ChangingStatus::DELETED;
        }) |
        views::transform([](const auto &ur) { return ur.roleId(); }) |
        ranges_utils::to<vector<int32_t>>();
    const auto newRoleIds =
        user.userRoles | views::filter([](const auto &ur) {
            return ur.changingStatus() == ChangingStatus::NEW;
        }) |
        views::transform([](const auto &ur) { return ur.roleId(); }) |
        ranges_utils::to<vector<int32_t>>();
    co_await roleVerifier_->verifyDeptRolesAllowedForNewUser(oldDeptId,
                                                             newDeptId,
                                                             allRoleIds,
                                                             newRoleIds);

    if (isUpdated)
    {
        user.markUpdatedBy(updatedBy);
        user.markUpdated();
    }
    co_return;
}

drogon::Task<> UserUpdater::updateBasicInfo(
    User &user,
    const UserInfoUpdateRequest &request) const
{
    bool isUpdated = false;
    ENTITY_SET(user, nickname, isUpdated = true);
    ENTITY_SET(user, sex, isUpdated = true);
    // 更新 phone_number
    if (request.phoneNumber() && user.phoneNumber != *request.phoneNumber())
    {
        user.phoneNumber = *request.phoneNumber();
        isUpdated = true;
    }
    // 置空 phone_number
    else if (request.phoneNumber().isNull() && user.phoneNumber)
    {
        user.phoneNumber = nullopt;
        isUpdated = true;
    }
    // 更新 email
    if (request.email() && user.email != *request.email())
    {
        user.email = *request.email();
        isUpdated = true;
    }
    // 置空 email
    else if (request.email().isNull() && user.email)
    {
        user.email = nullopt;
        isUpdated = true;
    }
    if (isUpdated)
    {
        user.markUpdatedBy(*user.userId);
        user.markUpdated();
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
        if (newRoleSet.find(ur.roleId()) == newRoleSet.end())
        {
            ur.markDeleted();
        }
    }

    // 准备已存在的角色id
    unordered_set<int32_t> existingRoleIds;
    for (const auto &rd : userRoles)
    {
        existingRoleIds.insert(rd.roleId());
    }

    for (int32_t roleId : newRoleIds)
    {
        // 没有在已拥有的角色id列表寻找到
        if (existingRoleIds.find(roleId) == existingRoleIds.end())
        {
            // 新增
            UserRole newUR(roleId, updatedBy);
            newUR.userId = userId;
            newUR.markNew();
            userRoles.push_back(std::move(newUR));
        }
    }
}
