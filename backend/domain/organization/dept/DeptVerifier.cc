#include "DeptVerifier.h"

#include "common/exception/BusinessException.h"
#include <unordered_map>

using namespace std;
using namespace drogon;
using namespace drogon::orm;

Task<> DeptVerifier::verifyDepartmentExists(const int32_t deptId) const
{
    const auto dept = co_await deptRepository_->getById(deptId);
    if (!dept)
    {
        throw BusinessException("指定的部门id不存在");
    }
}

Task<> DeptVerifier::verifyDeptNameNotDuplicated(
    const string &name,
    const optional<int32_t> &parentId) const
{
    const auto count =
        co_await deptRepository_->countNameByParentId(name, parentId);
    if (count > 0)
    {
        throw BusinessException("部门名称冲突");
    }
}

Task<> DeptVerifier::verifyNoSubDept(const int32_t deptId) const
{
    const auto count = co_await deptRepository_->countSubDept(deptId);
    if (count > 0)
    {
        throw BusinessException("指定部门包含子部门");
    }
}

// 没测，需要充分测试
Task<> DeptVerifier::verifyRoleAssignmentAllowed(
    const int32_t deptId,
    const vector<int32_t> roleIds) const
{
    LOG_TRACE << "验证部门分配角色是否合法，deptId=" << deptId
              << ", roleIds=" << roleIds.size() << "个角色";
    // 检查黑白名单
    co_await roleVerifier_->verifyRolesBelongToDept(deptId, roleIds);
    // 获取当前部门已经分配的每种角色的用户数量
    unordered_map<int32_t, size_t> roleCountMap =
        co_await userRepository_->countByDeptAndRoles(deptId, roleIds);

    // 获取当前部门限制的每种角色的用户数量
    const auto roleList = co_await roleRepository_->getByIds(roleIds, false);
    unordered_map<int32_t, size_t> roleLimitMap =
        roleList | views::transform([](const Role &role) {
            size_t limit = 0;
            if (role.getQuotaType() == QuotaType::Unlimited)
            {
                limit = SIZE_MAX;
            }
            else
            {
                // 总量限制不会超，这里不做区分
                limit = *role.getUserQuota();
            }
            return make_pair(*role.getRoleId(), limit);
        }) |
        ranges::to<unordered_map>();

    // 任意一种角色的用户数量大于等于限制的用户数量，抛异常
    for (const auto [roleId, count] : roleCountMap)
    {
        if (count >= roleLimitMap[roleId])
        {
            throw BusinessException("指定部门分配的角色已达用户数量限制");
        }
    }
}
