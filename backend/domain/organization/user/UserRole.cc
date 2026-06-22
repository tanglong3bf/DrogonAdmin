#include "UserRole.h"

#include "common/util/Utilities.hpp"

using namespace std;
using namespace trantor;

UserRole::UserRole(const int32_t roleId) : roleId_{roleId}
{
}

UserRole::UserRole(const int32_t roleId, int32_t createdBy)
    : roleId_{roleId}, createdBy{createdBy}, createdTime{Date::now()}
{
}

UserRole::UserRole(const SysUserRole &model)
    : INIT(userId, UserId),
      INIT(roleId_, RoleId),
      OPT_INIT(createdBy, CreatedBy),
      OPT_INIT(createdTime, CreatedTime)
{
}

UserRole::operator SysUserRole() const
{
    SysUserRole model;
    SET_OPT(userId, UserId);
    SET_VAL(roleId_, RoleId);
    SET_OPT(createdBy, CreatedBy);
    SET_OPT(createdTime, CreatedTime);
    return model;
}
