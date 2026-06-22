#include "RoleAssembler.h"

#include "common/exception/BusinessException.h"
#include "domain/authorization/Role.h"

using namespace drogon;

Task<Role> RoleAssembler::fromCreateRequest(const RoleCreateRequest request,
                                            std::int32_t createdBy)
{
    // 验证
    const auto name = request.getName();
    co_await roleVerifier_->verifyRoleNameNotDuplicated(name);
    const auto code = request.getCode();
    co_await roleVerifier_->verifyRoleCodeNotDuplicated(code);
    if (request.getRelationType() != RelationType::All)
    {
        if (request.getDeptIds().size() == 0)
        {
            throw BusinessException("请选择关联部门");
        }
        co_await deptVerifier_->verifyDeptIdsExist(request.getDeptIds());
    }

    // 必备参数
    Role role{name,
              code,
              request.getQuotaType(),
              request.getRelationType(),
              createdBy};

    // 可选参数
    if (const auto desc = request.getDescription())
    {
        role.setDescription(*desc);
    }
    if (role.getQuotaType() != QuotaType::Unlimited)
    {
        role.setUserQuota(*request.getUserQuota());
    }
    if (role.getRelationType() != RelationType::All)
    {
        for (const auto deptId : request.getDeptIds())
        {
            role.addRoleDept(RoleDept{deptId, createdBy});
        }
    }
    role.toNew();
    co_return role;
}
