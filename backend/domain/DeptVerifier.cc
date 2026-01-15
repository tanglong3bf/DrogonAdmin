#include "DeptVerifier.h"

#include "common/exception/BusinessException.h"

using namespace std;
using namespace drogon;
using namespace drogon::orm;

Task<> DeptVerifier::verifyDepartmentExists(const uint32_t deptId) const
{
    try
    {
        const auto dept = co_await deptRepository_->getById(deptId);
    }
    catch (const UnexpectedRows &e)
    {
        throw BusinessException("指定的父部门id不存在");
    }
}

Task<> DeptVerifier::verifyDeptNameNotDuplicated(
    const string &name,
    const optional<uint32_t> &parentId) const
{
    const auto count =
        co_await deptRepository_->countNameByParentId(name, parentId);
    if (count > 0)
    {
        throw BusinessException("部门名称冲突");
    }
}
