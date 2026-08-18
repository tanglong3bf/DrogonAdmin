#include "RoleUpdater.h"

#include "common/util/rangesUtils.hpp"
#include <ranges>

using namespace std;
using namespace drogon;
using namespace drogon_admin;

Task<> RoleUpdater::updateRole(Role &role,
                               const RoleUpdateRequest &request,
                               const int32_t updatedBy) const
{
    if (role.version() != request.version())
    {
        throw BusinessException{"更新期间数据发生变化，更新失败"};
    }
    const Role oldData = role;

    // 验证
    if (request.name() && role.name() != *request.name())
    {
        co_await roleVerifier_->verifyRoleNameNotDuplicated(*request.name());
    }
    if (request.code() && role.code() != *request.code())
    {
        co_await roleVerifier_->verifyRoleCodeNotDuplicated(*request.code());
    }

    // 基础信息更新
    role.updateBasicInfo(request.name(),
                         request.code(),
                         request.description(),
                         updatedBy);

    // 配额信息更新
    bool isQuotaUpdated = role.updateQuota(request.quotaType(),
                                           request.userQuota(),
                                           request.relationType(),
                                           updatedBy);

    // 关联部门
    if (request.deptIds() && request.deptIds()->size() > 0)
    {
        co_await deptVerifier_->verifyDeptIdsExist(*request.deptIds());

        isQuotaUpdated |= role.updateRoleDepts(*request.deptIds(), updatedBy);
    }

    // 关联关系
    if (role.relationType() != RelationType::All)
    {
        if (ranges::empty(role.roleDepts() | views::filter([](const auto &rd) {
                              return !rd.isDeleted();
                          })))
        {
            throw BusinessException("当relation_type不为0时，dept_ids不能为空");
        }
        vector<int32_t> deptIds =
            role.roleDepts() |
            views::filter([](const auto &rd) { return !rd.isDeleted(); }) |
            views::transform([](const auto &rd) { return rd.deptId(); }) |
            ranges_utils::to<vector>();
        if (role.relationType() == RelationType::Whitelist)
        {
            co_await userVerifier_->verifyUsersWithRoleBelongToDepts(
                *role.roleId(), deptIds);
        }
        else
        {
            co_await userVerifier_->verifyUsersWithRoleNotBelongToDepts(
                *role.roleId(), deptIds);
        }
    }

    if (isQuotaUpdated)
    {
        co_await roleVerifier_->checkQuota(role, oldData);
    }
}
