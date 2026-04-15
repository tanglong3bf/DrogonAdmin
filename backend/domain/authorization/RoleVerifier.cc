#include "RoleVerifier.h"

#include "common/exception/BusinessException.h"
#include <ranges>

using namespace std;
using namespace drogon;

Task<> RoleVerifier::verifyNoRolesBelongToDept(const int32_t deptId) const
{
    const auto count = co_await roleRepository_->countBelongDept(deptId);
    if (count > 0)
    {
        throw BusinessException("指定部门包含角色");
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
            return role.getRelationType() == RelationType::Whitelist;
        }) |
        ranges::to<vector>();

    const auto hasBlackList =
        roles | views::filter([](const Role &role) {
            return role.getRelationType() == RelationType::Blacklist;
        }) |
        ranges::to<vector>();

    for (const auto &role : hasWhiteList)
    {
        const auto &deptIds = role.getRoleDepts() |
                              views::transform([](const RoleDept &roleDept) {
                                  return roleDept.getDeptId();
                              }) |
                              ranges::to<vector>();
        if (find(deptIds.begin(), deptIds.end(), deptId) == deptIds.end())
        {
            throw BusinessException("指定部门不在角色的白名单中");
        }
    }

    for (const auto &role : hasBlackList)
    {
        const auto &deptIds = role.getRoleDepts() |
                              views::transform([](const RoleDept &roleDept) {
                                  return roleDept.getDeptId();
                              }) |
                              ranges::to<vector>();
        if (std::find(deptIds.begin(), deptIds.end(), deptId) != deptIds.end())
        {
            throw BusinessException("指定部门在角色的黑名单中");
        }
    }
}
