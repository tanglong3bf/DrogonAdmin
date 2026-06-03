#include "RoleService.h"

using namespace std;
using namespace drogon;
using namespace drogon::orm;

Task<> RoleService::deleteExcludingDept(const std::int32_t deptId,
                                        const DbClientPtr &dbClient) const
{
    auto roleDepts =
        co_await roleRepository_->getExcludingDeptByDeptId(deptId, dbClient);
    for (auto &roleDept : roleDepts)
    {
        roleDept.markDeleted();
    }
    co_await roleRepository_->saveRoleDepts(roleDepts, dbClient);
}

Task<PaginatedResponse<RoleResponse>> RoleService::getRoleList(
    const RoleQueryRequest &request) const
{
    const size_t count =
        co_await roleCqrsRepo_->countByNameAndDeptId(request.name(),
                                                     request.deptId());

    if (count == 0)
    {
        co_return PaginatedResponse<RoleResponse>{request.page(),
                                                  request.pageSize(),
                                                  0,
                                                  {}};
    }

    const size_t maxPage =
        (count + request.pageSize() - 1) / request.pageSize();
    const auto list = co_await roleCqrsRepo_->getRoleList(request, maxPage);

    co_return PaginatedResponse<RoleResponse>{maxPage < request.page()
                                                  ? maxPage
                                                  : request.page(),
                                              request.pageSize(),
                                              count,
                                              list};
}

Task<> RoleService::createRole(const RoleCreateRequest &request,
                               const std::int32_t createdBy) const
{
    auto role = co_await roleAssembler_->fromCreateRequest(request, createdBy);
    co_await roleRepository_->save(role);
}

Task<> RoleService::updateRole(const std::int32_t roleId,
                               const RoleUpdateRequest request,
                               const std::int32_t updatedBy) const
{
    LOG_TRACE << "更新角色，roleId=" << roleId << ", updatedBy=" << updatedBy;
    auto role = co_await roleRepository_->getById(roleId);
    co_await roleUpdater_->updateRole(role, request, updatedBy);
    co_await roleRepository_->save(role);
}

Task<> RoleService::deleteRole(const std::int32_t roleId,
                               const std::int32_t deletedBy) const
{
    auto role = co_await roleRepository_->getById(roleId);
    co_await roleHandler_->deleteRole(role, deletedBy);
    co_await roleRepository_->save(role);
}

Task<vector<AssignableRoleResponse>> RoleService::getAssignableRoles(
    const std::int32_t deptId) const
{
    co_return co_await roleCqrsRepo_->getAssignableRoles(deptId);
}
