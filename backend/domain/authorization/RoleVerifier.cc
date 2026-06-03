#include "RoleVerifier.h"

#include "common/exception/BusinessException.h"
#include <ranges>
#include <unordered_map>

using namespace std;
using namespace drogon;

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

drogon::Task<> RoleVerifier::verifyRolesBelongToDept(
    const std::int32_t deptId,
    const std::vector<std::int32_t> &roleIds) const
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
        ranges::to<vector>();

    const auto hasBlackList =
        roles | views::filter([](const Role &role) {
            return role.relationType == RelationType::Blacklist;
        }) |
        ranges::to<vector>();

    for (const auto &role : hasWhiteList)
    {
        const auto &deptIds = role.roleDepts |
                              views::transform([](const RoleDept &roleDept) {
                                  return roleDept.deptId();
                              }) |
                              ranges::to<vector>();
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
                              ranges::to<vector>();
        if (std::find(deptIds.begin(), deptIds.end(), deptId) != deptIds.end())
        {
            throw BusinessException("指定部门在角色的黑名单中");
        }
    }
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
                         ranges::to<vector>();
    if (role.quotaType == QuotaType::TotalLimit)
    {
        // 检查所有使用当前角色的用户数量是否超过限制
        const auto count =
            co_await userRepository_->countByRoleId(*role.roleId);
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
