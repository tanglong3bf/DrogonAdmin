#include "DeptHandler.h"

#include "common/exception/BusinessException.h"

using namespace std;
using namespace drogon;
using namespace drogon::orm;

Task<> DeptHandler::updateDept(Dept &dept,
                               const string &newName,
                               const int32_t updatedBy) const
{
    validateNameNotSame(dept.getName(), newName);
    co_await deptVerifier_->verifyDeptNameNotDuplicated(newName,
                                                        dept.getParentId());

    dept.setName(newName);
    dept.setUpdatedBy(updatedBy);
}

void DeptHandler::validateNameNotSame(const string &oldName,
                                      const string &newName) const
{
    if (oldName == newName)
    {
        throw BusinessException{"部门新名称和旧名称相同"};
    }
}

drogon::Task<> DeptHandler::deleteDept(Dept &dept,
                                       const int32_t deletedBy) const
{
    co_await deptVerifier_->verifyNoSubDept(*dept.getDeptId());
    co_await userVerifier_->verifyNoUserInDept(*dept.getDeptId());
    co_await roleVerifier_->ensureNoRolesBelongToDept(*dept.getDeptId());

    dept.setDeletedBy(deletedBy);
}
