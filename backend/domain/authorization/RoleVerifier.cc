#include "RoleVerifier.h"

#include "common/exception/BusinessException.h"

using namespace std;
using namespace drogon;

Task<> RoleVerifier::ensureNoRolesBelongToDept(const int32_t deptId) const
{
    const auto count = co_await roleRepository_->countBelongDept(deptId);
    if (count > 0)
    {
        throw BusinessException("指定部门包含用户");
    }
}
