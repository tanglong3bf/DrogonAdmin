#include "RoleService.h"

using namespace std;
using namespace drogon;

Task<> RoleService::deleteExcludingDept(const int32_t deptId,
                                        const int32_t deletedBy) const
{
    auto roleDepts = co_await roleRepository_->getExcludingDeptByDeptId(deptId);
    for (auto &roleDept : roleDepts)
    {
        roleDept.setDeletedBy(deletedBy);
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
