#include "RoleHandler.h"

drogon::Task<> RoleHandler::deleteRole(Role &role, int32_t deletedBy) const
{
    co_await userVerifier_->verifyRoleNotUsed(*role.getRoleId());
    role.setDeletedBy(deletedBy);
    for (auto roleDept : role.getDepts())
    {
        roleDept.toDelete();
    }
}
