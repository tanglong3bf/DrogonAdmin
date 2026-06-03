#include "UserUpdater.h"

#include "common/exception/BusinessException.h"
#include <unordered_set>

using namespace std;
using namespace drogon;

Task<> UserUpdater::updateUser(User &user,
                               const UserUpdateRequest &request,
                               const int32_t updatedBy) const
{
    LOG_TRACE << "更新用户，userId=" << *user.userId
              << ", updatedBy=" << updatedBy;
    bool isUpdated = false;
    bool deptIdUpdated = false;
    ENTITY_SET(user, nickname, isUpdated = true);
    ENTITY_SET(user, sex, isUpdated = true);
    ENTITY_SET(user, deptId, isUpdated = true; deptIdUpdated = true);
    ENTITY_SET(user, phoneNumber, isUpdated = true);
    ENTITY_SET(user, email, isUpdated = true);
    ENTITY_SET(user, status, isUpdated = true);

    // 用户现有的角色id列表
    const auto oldRoleIds =
        user.userRoles |
        views::transform([](const UserRole &ur) { return ur.roleId(); }) |
        ranges::to<vector>();
    // 需要新增的角色id列表
    const auto needInsertRoleIds =
        *request.roleIds() | views::filter([&](int32_t id) {
            return find(oldRoleIds.begin(), oldRoleIds.end(), id) ==
                   oldRoleIds.end();
        }) |
        ranges::to<vector>();
    // 需要删除的角色id列表
    const auto needDeleteRoleIds =
        oldRoleIds | views::filter([&](int32_t id) {
            return find(request.roleIds()->begin(),
                        request.roleIds()->end(),
                        id) == request.roleIds()->end();
        }) |
        ranges::to<vector>();

    if (deptIdUpdated || needInsertRoleIds.size() > 0 ||
        needDeleteRoleIds.size() > 0)
    {
        vector<int> roleIds = *request.roleIds();
        sort(roleIds.begin(), roleIds.end());
        // 检查是否可以为指定部门分配这些角色
        if (deptIdUpdated)
        {
            co_await deptVerifier_->verifyRoleAssignmentAllowed(user.deptId,
                                                                roleIds);
        }
        else if (needInsertRoleIds.size() > 0)
        {
            co_await deptVerifier_->verifyRoleAssignmentAllowed(
                user.deptId, needInsertRoleIds);
        }

        updateUserRoles(const_cast<vector<UserRole> &>(user.userRoles),
                        roleIds,
                        *user.userId,
                        updatedBy);

        isUpdated = true;
    }

    if (isUpdated)
    {
        user.markUpdatedBy(updatedBy);
        user.markUpdated();
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
        if (newRoleSet.find(ur.roleId()) == newRoleSet.end())
        {
            ur.markDeleted();
        }
    }

    unordered_set<int32_t> existingRoleIds;
    for (const auto &rd : userRoles)
    {
        existingRoleIds.insert(rd.roleId());
    }

    for (int32_t roleId : newRoleIds)
    {
        if (existingRoleIds.find(roleId) == existingRoleIds.end())
        {
            UserRole newUR(roleId, updatedBy);
            newUR.userId = userId;
            newUR.markNew();
            userRoles.push_back(std::move(newUR));
        }
    }
}
