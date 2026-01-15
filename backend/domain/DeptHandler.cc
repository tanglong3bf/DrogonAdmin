#include "DeptHandler.h"

#include "common/exception/BusinessException.h"

using namespace std;
using namespace drogon;
using namespace drogon::orm;

Task<> DeptHandler::updateDept(Dept &dept,
                               const string &newName,
                               const uint32_t updatedBy) const
{
    validateNameNotSame(dept.getName(), newName);
    // 验证新名称不和其他部门冲突
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
