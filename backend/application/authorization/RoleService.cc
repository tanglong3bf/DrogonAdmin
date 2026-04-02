#include "RoleService.h"

using namespace std;
using namespace drogon;

Task<> RoleService::deleteExcludingDept(const int32_t deptId,
                                        const int32_t deletedBy) const
{
    auto roleDepts = co_await roleRepository_->getExcludingDeptByDeptId(deptId);
    for (auto &roleDept : roleDepts)
    {
        roleDept.toDelete();
    }
    co_await roleRepository_->saveRoleDepts(roleDepts);
}

Task<PaginatedResponse<RoleResponse>> RoleService::getRoleList(
    const GetRoleListRequest &request) const
{
    const int32_t count =
        co_await roleCqrsRepo_->countByNameAndDeptId(request.getName(),
                                                     request.getDeptId());

    if (count == 0)
    {
        co_return PaginatedResponse<RoleResponse>{request.getPage(),
                                                  request.getPageSize(),
                                                  0,
                                                  {}};
    }

    const size_t maxPage =
        (count + request.getPageSize() - 1) / request.getPageSize();
    const auto list = co_await roleCqrsRepo_->getRoleList(request, maxPage);

    co_return PaginatedResponse<RoleResponse>{request.getPage(),
                                              request.getPageSize(),
                                              count,
                                              list};
}

Task<> RoleService::createRole(const RoleCreateRequest &request,
                               const int32_t createdBy) const
{
    auto role = co_await roleAssembler_->fromCreateRequest(request, createdBy);
    role.toNew();
    co_await roleRepository_->save(role);
}

Task<> RoleService::updateRole(const std::int32_t roleId,
                               const RoleUpdateRequest request,
                               const std::int32_t updatedBy) const
{
    auto role = co_await roleRepository_->getById(roleId);
    co_await roleUpdater_->updateRole(role, request, updatedBy);
    role.toUpdate();
    co_await roleRepository_->save(role);
}

Task<> RoleService::deleteRole(const std::int32_t roleId,
                               const std::int32_t deletedBy) const
{
    auto role = co_await roleRepository_->getById(roleId);
    co_await roleHandler_->deleteRole(role, deletedBy);
    role.toDelete();
    co_await roleRepository_->save(role);
}
