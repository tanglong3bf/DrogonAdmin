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
