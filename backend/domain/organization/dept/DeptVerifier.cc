#include "DeptVerifier.h"

#include "common/exception/BusinessException.h"

using int32_t = std::int32_t;
using namespace std;
using namespace drogon;
using namespace drogon::orm;

Task<> DeptVerifier::verifyDepartmentExists(const int32_t deptId) const
{
    try
    {
        const auto dept = co_await deptRepository_->getById(deptId);
    }
    catch (const UnexpectedRows &e)
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

// TODO: 逻辑略复杂，先留着
Task<> DeptVerifier::verifyRoleAssignmentAllowed(
    const int32_t deptId,
    const vector<int32_t> roleIds) const
{
    // 仅考虑黑白名单情况，两种情况不允许分配角色
    // 出现这两种情况，直接抛异常
    //// 1. 角色有白名单，但不包含指定部门
    //// 2. 角色有黑名单，并且包含指定部门

    // 获取当前部门已经分配的每种角色的用户数量

    // 获取当前部门限制的每种角色的用户数量

    // 任意一种角色的用户数量大于等于限制的用户数量，抛异常
    co_return;
}
