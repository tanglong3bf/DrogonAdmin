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
    : INIT(userId, UserId), INIT(roleId, RoleId)
{
}

UserRole::operator SysUserRole() const
{
    SysUserRole model;
    if (userId_)
    {
        model.setUserId(*userId_);
    }
    model.setRoleId(roleId_);
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
