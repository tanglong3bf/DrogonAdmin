#include "RoleHandler.h"

using namespace drogon;

Task<> RoleHandler::deleteRole(Role &role, int32_t deletedBy) const
{
    co_await userVerifier_->verifyRoleNotUsed(*role.roleId());
    role.remove(deletedBy);
}
