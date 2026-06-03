#include "UserAssembler.h"

using namespace std;
using namespace drogon;

Task<User> UserAssembler::fromCreateRequest(const UserCreateRequest &request,
                                            const int32_t createdBy) const
{
    // 验证
    co_await userVerifier_->verifyUsernameNotDuplicated(
        static_cast<string>(request.username()));
    co_await userVerifier_->verifyNicknameNotDuplicated(
        static_cast<string>(request.nickname()));
    co_await deptVerifier_->verifyDepartmentExists(request.deptId());
    if (request.roleIds() && request.roleIds()->size() > 0)
    {
        co_await deptVerifier_->verifyRoleAssignmentAllowed(request.deptId(),
                                                            *request.roleIds());
    }

    // 必备参数
    User user{static_cast<string>(request.username()),
              static_cast<string>(request.nickname()),
              request.sex(),
              request.deptId(),
              request.status(),
              createdBy};

    // 可选参数
    if (const auto phoneNumber = request.phoneNumber())
    {
        user.phoneNumber = *phoneNumber;
    }
    if (const auto email = request.email())
    {
        user.email = *email;
    }
    if (const auto roleIds = request.roleIds())
    {
        for (const auto roleId : *roleIds)
        {
            user.addUserRole(UserRole{roleId, createdBy});
        }
    }
    user.markNew();
    co_return user;
}
