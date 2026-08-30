#include "RoleHandler.h"

using namespace drogon;

Task<> RoleHandler::deleteRole(Role &role,
                               const int32_t version,
                               const int32_t deletedBy) const
{
    if (version != role.version())
    {
        throw BusinessException("删除期间数据发生变化，删除失败");
    }
    co_await userVerifier_->verifyRoleNotUsed(*role.roleId());
    role.remove(deletedBy);
}
