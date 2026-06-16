#include "DeptVerifier.h"

#include "common/exception/BusinessException.h"

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

Task<> DeptVerifier::verifyDeptIdsExist(
    const vector<std::int32_t> deptIds) const
{
    const auto dept = co_await deptRepository_->getByIds(deptIds);
    if (dept.size() != deptIds.size())
    {
        throw BusinessException("部分部门id不存在");
    }
}
