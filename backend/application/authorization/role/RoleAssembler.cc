#include "RoleAssembler.h"

#include "common/exception/BusinessException.h"
#include "domain/authorization/Role.h"

using namespace drogon;

Task<Role> RoleAssembler::fromCreateRequest(const RoleCreateRequest request,
                                            std::int32_t createdBy)
{
    // 验证
    const std::string name{request.name()};
    co_await roleVerifier_->verifyRoleNameNotDuplicated(name);
    const std::string code{request.code()};
    co_await roleVerifier_->verifyRoleCodeNotDuplicated(code);
    if (request.relationType() != RelationType::All)
    {
        if (request.deptIds().size() == 0)
        {
            throw BusinessException("请选择关联部门");
        }
        co_await deptVerifier_->verifyDeptIdsExist(request.deptIds());
    }

    // 必备参数
    Role role{
        name, code, request.quotaType(), request.relationType(), createdBy};

    // 可选参数
    if (const auto desc = request.description())
    {
        role.description = *desc;
    }
    if (role.quotaType != QuotaType::Unlimited)
    {
        role.userQuota = *request.userQuota();
    }
    if (role.relationType != RelationType::All)
    {
        for (const auto deptId : request.deptIds())
        {
            role.addRoleDept(RoleDept{deptId, createdBy});
        }
    }
    role.markNew();
    co_return role;
}
