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
    : INIT(roleId_, RoleId),
      INIT(deptId_, DeptId),
      OPT_INIT(createdBy_, CreatedBy),
      OPT_INIT(createdTime_, CreatedTime)
{
}

RoleDept::operator SysRoleDept() const
{
    SysRoleDept model;
    SET_OPT(roleId_, RoleId);
    SET_VAL(deptId_, DeptId);
    SET_OPT(createdBy_, CreatedBy);
    SET_OPT(createdTime_, CreatedTime);
    return model;
}
