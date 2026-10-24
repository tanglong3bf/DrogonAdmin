#include "UserAssembler.h"

#include "BCryptCpp/BCrypt.h"
#include "common/util/rangesUtils.hpp"

using namespace std;
using namespace drogon;
using namespace BCryptCpp;
using namespace drogon_admin;

Task<User> UserAssembler::fromCreateRequest(const UserCreateRequest &request,
                                            const int32_t createdBy) const
{
    // 验证
    co_await userVerifier_->verifyUsernameNotDuplicated(
        string(request.username()));
    co_await userVerifier_->verifyNicknameNotDuplicated(
        string(request.nickname()));
    co_await deptVerifier_->verifyDepartmentExists(request.deptId());
    if (request.roleIds())
    {
        co_await roleVerifier_->verifyRolesExists(*request.roleIds());
        if (request.roleIds()->size() > 0)
        {
            co_await roleVerifier_->verifyDeptRolesAllowedForNewUser(
                request.deptId(), *request.roleIds());
        }
    }

    // 准备默认密码
    const auto salt = BCrypt::GenerateSalt();
    const auto hashedPassword = BCrypt::HashPassword("123456", salt);
    // 必备参数
    User user{request.username(),
              hashedPassword,
              request.nickname(),
              request.sex(),
              request.deptId(),
              request.status(),
              createdBy};

    // 可选参数
    user.constructOptionalFields(request.phoneNumber(), request.email());
    if (const auto roleIds = request.roleIds())
    {
        user.appendRoles(*roleIds, createdBy);
    }
    user.markNew();
    co_return user;
}
