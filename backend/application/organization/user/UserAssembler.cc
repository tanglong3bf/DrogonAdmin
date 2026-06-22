#include "UserAssembler.h"

using namespace drogon;

Task<User> UserAssembler::fromCreateRequest(const UserCreateRequest &request,
                                            const int32_t createdBy) const
{
    // 验证
    co_await userVerifier_->verifyUsernameNotDuplicated(request.getUsername());
    co_await userVerifier_->verifyNicknameNotDuplicated(request.getNickname());
    co_await deptVerifier_->verifyDepartmentExists(request.getDeptId());
    if (request.getRoleIds())
    {
        co_await roleVerifier_->verifyRolesExists(*request.getRoleIds());
        if (request.getRoleIds()->size() > 0)
        {
            co_await roleVerifier_->verifyDeptRolesAllowedForNewUser(
                request.getDeptId(), *request.getRoleIds());
        }
    }

    // 必备参数
    User user{request.getUsername(),
              request.getNickname(),
              request.getSex(),
              request.getDeptId(),
              request.getStatus(),
              createdBy};

    // 可选参数
    if (const auto phoneNumber = request.getPhoneNumber())
    {
        user.setPhoneNumber(*phoneNumber);
    }
    if (const auto email = request.getEmail())
    {
        user.setEmail(*email);
    }
    if (const auto roleIds = request.getRoleIds())
    {
        for (const auto roleId : *roleIds)
        {
            user.addUserRole(UserRole{roleId, createdBy});
        }
    }
    user.toNew();
    co_return user;
}
