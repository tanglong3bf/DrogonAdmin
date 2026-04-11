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
    SET_OPT(roleId, RoleId);
    SET_VAL(deptId, DeptId);
    SET_OPT(createdBy, CreatedBy);
    SET_OPT(createdTime, CreatedTime);
    return model;
}
