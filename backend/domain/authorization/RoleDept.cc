#include "RoleDept.h"

#include "common/util/Utilities.hpp"

RoleDept::RoleDept(const std::int32_t deptId) : deptId_{deptId}
{
}

RoleDept::RoleDept(const std::int32_t deptId, int32_t createdBy)
    : deptId_{deptId},
      AuditableEntity{createdBy,
                      trantor::Date::now(),
                      createdBy,
                      trantor::Date::now()}
{
}

RoleDept::RoleDept(const SysRoleDept &model)
    : INIT(id, Id), INIT(roleId, RoleId), INIT(deptId, DeptId)
{
}

RoleDept::operator SysRoleDept() const
{
    SysRoleDept model;
    if (id_)
    {
        model.setId(*id_);
    }
    if (roleId_)
    {
        model.setRoleId(*roleId_);
    }
    model.setDeptId(deptId_);
    model.setCreatedBy(*createdBy_);
    model.setCreatedTime(*createdTime_);
    model.setUpdatedBy(*updatedBy_);
    model.setUpdatedTime(*updatedTime_);
    if (deletedBy_)
    {
        model.setDeletedBy(*deletedBy_);
    }
    if (deletedTime_)
    {
        model.setDeletedTime(*deletedTime_);
    }
    return model;
}
