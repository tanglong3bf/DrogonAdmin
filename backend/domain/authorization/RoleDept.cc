#include "RoleDept.h"

RoleDept::RoleDept(const SysRoleDept &sysRoleDept)
    : id_{sysRoleDept.getValueOfId()},
      roleId_{sysRoleDept.getValueOfRoleId()},
      deptId_{sysRoleDept.getValueOfDeptId()},
      relationType_{static_cast<RoleDeptRelationType>(
          sysRoleDept.getValueOfRelationType())}
{
}

RoleDept::operator SysRoleDept() const
{
    SysRoleDept result;
    return result;
}
