#include "UserVerifier.h"

#include "common/exception/BusinessException.h"

using namespace std;
using namespace drogon;

Task<> UserVerifier::verifyNoUserInDept(const int32_t deptId) const
{
    const auto count = co_await userRepository_->countByDept(deptId);
    if (count > 0)
    {
        throw BusinessException("指定部门包含用户");
    }
}

Task<> UserVerifier::verifyRoleNotUsed(const int32_t roleId) const
{
    const auto count = co_await userRepository_->countByRole(roleId);
    if (count > 0)
    {
        throw BusinessException("指定角色被用户使用");
    }
}

Task<> UserVerifier::verifyUsernameNotDuplicated(const string &username) const
{
    const auto count = co_await userRepository_->countByUsername(username);
    if (count > 0)
    {
        throw BusinessException("指定用户名已被使用");
    }
}

Task<> UserVerifier::verifyNicknameNotDuplicated(const string &nickname) const
{
    const auto count = co_await userRepository_->countByNickname(nickname);
    if (count > 0)
    {
        throw BusinessException("指定昵称已被使用");
    }
}
