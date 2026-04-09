#include "RoleDept.h"

#include "common/util/Utilities.hpp"

using namespace std;
using namespace trantor;

RoleDept::RoleDept(const int32_t deptId) : deptId_{deptId}
{
}

RoleDept::RoleDept(const int32_t deptId, int32_t createdBy)
    : deptId_{deptId}, createdBy_{createdBy}, createdTime_{Date::now()}
{
}

RoleDept::RoleDept(const SysRoleDept &model)
    : INIT(roleId, RoleId), INIT(deptId, DeptId)
{
}

RoleDept::operator SysRoleDept() const
{
    SysRoleDept model;
    if (roleId_)
    {
        model.setRoleId(*roleId_);
    }
    model.setDeptId(deptId_);
    if (createdBy_)
    {
        model.setCreatedBy(*createdBy_);
    }
    if (createdTime_)
    {
        model.setCreatedTime(*createdTime_);
    }
    return model;
}
