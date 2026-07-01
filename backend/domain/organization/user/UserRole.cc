#include "UserRole.h"

#include "common/util/Utilities.hpp"

using namespace std;
using namespace trantor;

UserRole::UserRole(const int32_t roleId) : roleId_{roleId}
{
}

UserRole::UserRole(const int32_t roleId, int32_t createdBy)
    : roleId_{roleId}, createdBy_{createdBy}, createdTime_{Date::now()}
{
}

UserRole::UserRole(const SysUserRole &model)
    : INIT(userId_, UserId),
      INIT(roleId_, RoleId),
      OPT_INIT(createdBy_, CreatedBy),
      OPT_INIT(createdTime_, CreatedTime)
{
}

UserRole::operator SysUserRole() const
{
    SysUserRole model;
    SET_OPT(userId_, UserId);
    SET_VAL(roleId_, RoleId);
    SET_OPT(createdBy_, CreatedBy);
    SET_OPT(createdTime_, CreatedTime);
    return model;
}
