#include "RoleVerifier.h"

#include "common/exception/BusinessException.h"

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
