#include "UserUpdater.h"

#include "common/util/rangesUtils.hpp"
#include <ranges>

using namespace std;
using namespace drogon;
using namespace drogon_admin;

Task<> UserUpdater::updateUser(User &user,
                               const UserUpdateRequest &request,
                               const int32_t updatedBy) const
{
    LOG_TRACE << "更新用户，userId=" << *user.userId()
              << ", updatedBy=" << updatedBy;

    bool isUpdated = user.updateBasicInfo(request.nickname(),
                                          request.sex(),
                                          request.phoneNumber(),
                                          request.email(),
                                          updatedBy);

    // 状态
    if (const auto statusOpt = request.status(); statusOpt)
    {
        isUpdated |= user.updateStatus(*statusOpt, updatedBy);
    }

    // 部门id
    const auto oldDeptId = user.deptId();
    if (const auto deptIdOpt = request.deptId();
        deptIdOpt && user.deptId() != *deptIdOpt)
    {
        co_await deptVerifier_->verifyDepartmentExists(*deptIdOpt);
        isUpdated |= user.assignToDept(*deptIdOpt, updatedBy);
    }
    const auto newDeptId = user.deptId();

    // 角色列表
    if (request.roleIds())
    {
        co_await roleVerifier_->verifyRolesExists(*request.roleIds());
        user.replaceRoles(*request.roleIds(), updatedBy);
        isUpdated = true;
    }
    const auto allRoleIds =
        user.userRoles() | views::filter([](const auto &ur) {
            return ur.changingStatus() != ChangingStatus::DELETED;
        }) |
        views::transform([](const auto &ur) { return ur.roleId(); }) |
        ranges_utils::to<vector<int32_t>>();
    const auto newRoleIds =
        user.userRoles() | views::filter([](const auto &ur) {
            return ur.changingStatus() == ChangingStatus::NEW;
        }) |
        views::transform([](const auto &ur) { return ur.roleId(); }) |
        ranges_utils::to<vector<int32_t>>();

    co_await roleVerifier_->verifyDeptRolesAllowedForNewUser(oldDeptId,
                                                             newDeptId,
                                                             allRoleIds,
                                                             newRoleIds);
}
