#include "UserVerifier.h"

#include "common/exception/BusinessException.h"

drogon::Task<> UserVerifier::verifyNoUserInDept(const std::int32_t deptId) const
{
    const auto count = co_await userRepository_->countByDept(deptId);
    if (count > 0)
    {
        throw BusinessException("指定部门包含用户");
    }
}

drogon::Task<> UserVerifier::verifyRoleNotUsed(const std::int32_t roleId) const
{
    const auto count = co_await userRepository_->countByRole(roleId);
    if (count > 0)
    {
        throw BusinessException("指定角色被用户使用");
    }
}
