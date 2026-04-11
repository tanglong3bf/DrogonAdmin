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
    LOG_WARN << "逻辑略复杂，先留着。";
    co_return;
}
