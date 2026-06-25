#include "RoleVerifier.h"

#include "common/exception/BusinessException.h"
#include "common/util/rangesUtils.hpp"

using namespace std;
using namespace drogon;
using namespace tl;

Task<> RoleVerifier::verifyNoRolesBelongToDept(const int32_t deptId) const
{
    const auto count = co_await roleRepository_->countBelongDept(deptId);
    if (count > 0)
    {
        throw BusinessException("指定部门包含在角色的白名单中，无法删除");
    }
}

Task<> RoleVerifier::verifyRoleNameNotDuplicated(const string &name) const
{
    const auto count = co_await roleRepository_->countByName(name);
    if (count > 0)
    {
        throw BusinessException("指定角色名称已被使用");
    }
}

Task<> RoleVerifier::verifyRoleCodeNotDuplicated(const string &code) const
{
    const auto count = co_await roleRepository_->countByCode(code);
    if (count > 0)
    {
        throw BusinessException("指定角色代码已被使用");
    }
}

Task<> RoleVerifier::verifyRolesExists(const vector<int32_t> &roleIds) const
{
    const auto count = co_await roleRepository_->countByIds(roleIds);
    if (count != roleIds.size())
    {
        throw BusinessException("部分角色不存在");
    }
}

Task<> RoleVerifier::verifyRolesBelongToDept(
    const int32_t deptId,
    const vector<int32_t> &roleIds) const
{
    // 仅考虑黑白名单情况，两种情况不允许分配角色
    // 出现这两种情况，直接抛异常
    //// 1. 角色有白名单，但不包含指定部门
    //// 2. 角色有黑名单，并且包含指定部门
    const auto roles = co_await roleRepository_->getByIds(roleIds, true);

    const auto hasWhiteList =
        roles | views::filter([](const Role &role) {
            return role.relationType == RelationType::Whitelist;
        }) |
        ranges_utils::to<vector>();

    const auto hasBlackList =
        roles | views::filter([](const Role &role) {
            return role.relationType == RelationType::Blacklist;
        }) |
        ranges_utils::to<vector>();

    for (const auto &role : hasWhiteList)
    {
        const auto &deptIds = role.roleDepts |
                              views::transform([](const RoleDept &roleDept) {
                                  return roleDept.deptId();
                              }) |
                              ranges_utils::to<vector>();
        if (find(deptIds.begin(), deptIds.end(), deptId) == deptIds.end())
        {
            throw BusinessException("指定部门不在角色的白名单中");
        }
    }

    for (const auto &role : hasBlackList)
    {
        const auto &deptIds = role.roleDepts |
                              views::transform([](const RoleDept &roleDept) {
                                  return roleDept.deptId();
                              }) |
                              ranges_utils::to<vector>();
        if (std::find(deptIds.begin(), deptIds.end(), deptId) != deptIds.end())
        {
            throw BusinessException("指定部门在角色的黑名单中");
        }
    }
}

Task<> RoleVerifier::verifyDeptRolesAllowedForNewUser(
    const int32_t oldDeptId,
    const int32_t newDeptId,
    const vector<int32_t> &allRoleIds,
    const vector<int32_t> &newRoleIds) const
{
    const bool isSameDept = oldDeptId == newDeptId;
    const auto roleIds = isSameDept ? newRoleIds : allRoleIds;

    // 验证指定部门可用所有角色
    co_await verifyRolesBelongToDept(newDeptId, roleIds);

    const auto rolesInDb = co_await roleRepository_->getByIds(roleIds, true);
    // 获取每个角色在指定部门内已经使用的用户数量
    const map<int32_t, size_t> userCount =
        co_await userRepository_->countUsersPerRoleInDepartment(newDeptId,
                                                                roleIds);

    for (const auto &role : rolesInDb | views::filter([](const Role &role) {
                                return role.quotaType ==
                                       QuotaType::PerDeptLimit;
                            }) | ranges_utils::to<vector>())
    {
        // 检查每个部门使用当前角色的用户数量是否超过限制
        const size_t userCountInRole =
            userCount.contains(*role.roleId) ? userCount.at(*role.roleId) : 0;
        if (userCountInRole + 1 > *role.userQuota)
        {
            throw BusinessException("使用当前角色的用户数量已达限制");
        }
    }

    const auto totalLimitRoles =
        rolesInDb | views::filter([](const Role &role) {
            return role.quotaType == QuotaType::TotalLimit;
        }) |
        ranges_utils::to<vector>();
    auto totalLimitUserCount = co_await userRepository_->countByRoleList(
        totalLimitRoles |
        views::transform([](const Role &role) { return *role.roleId; }) |
        ranges_utils::to<vector>());

    for (const auto &role : totalLimitRoles)
    {
        // 检查所有使用当前角色的用户数量是否超过限制
        if (totalLimitUserCount[*role.roleId] + 1 > *role.userQuota)
        {
            throw BusinessException("使用当前角色的用户数量已达限制");
        }
    }
}

Task<> RoleVerifier::verifyDeptRolesAllowedForNewUser(
    const int32_t deptId,
    const vector<int32_t> &roleIds) const
{
    co_await verifyDeptRolesAllowedForNewUser(0, deptId, roleIds, {});
}

// QuotaType // 无数量限制、总量限制、每部门限制
// UserQuota // 数量
// RelationType // 所有部门、白名单、黑名单
// RoleDepts // 具体的部门列表
Task<> RoleVerifier::checkQuota(const Role &role,
                                const Role & /* ignore */) const
{
    const auto deptIds = role.roleDepts |
                         views::transform([](const RoleDept &roleDept) {
                             return roleDept.deptId();
                         }) |
                         ranges_utils::to<vector>();
    if (role.quotaType == QuotaType::TotalLimit)
    {
        // 检查所有使用当前角色的用户数量是否超过限制
        const auto count = co_await userRepository_->countByRole(*role.roleId);
        if (count > *role.userQuota)
        {
            throw BusinessException("使用当前角色的用户数量已达限制");
        }
    }
    else if (role.quotaType == QuotaType::PerDeptLimit)
    {
        // 检查每个部门使用当前角色的用户数量是否超过限制
        const std::unordered_map<std::int32_t, std::size_t> count =
            co_await userRepository_->countUsersWithRolePerDepartment(
                *role.roleId);
        for (const auto &[deptId, deptCount] : count)
        {
            if (deptCount > *role.userQuota)
            {
                throw BusinessException("使用当前角色的用户数量已达限制");
            }
        }
    }
    // 使用当前角色的用户所在的部门id列表
    const std::vector<std::int32_t> usedDeptIds =
        co_await userRepository_->getDeptIdsByRoleId(*role.roleId);
    if (role.relationType == RelationType::Whitelist)
    {
        // 检查所有使用当前角色的用户是否都在白名单部门中
        for (const auto &deptId : usedDeptIds)
        {
            if (find(deptIds.begin(), deptIds.end(), deptId) == deptIds.end())
            {
                throw BusinessException("使用当前角色的用户不在白名单部门中");
            }
        }
    }
    else if (role.relationType == RelationType::Blacklist)
    {
        // 检查所有使用当前角色的用户是否都不在黑名单部门中
        for (const auto &deptId : usedDeptIds)
        {
            if (find(deptIds.begin(), deptIds.end(), deptId) != deptIds.end())
            {
                throw BusinessException("使用当前角色的用户在黑名单部门中");
            }
        }
    }
}
