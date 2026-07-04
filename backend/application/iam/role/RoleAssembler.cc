#include "RoleAssembler.h"

#include "common/exception/BusinessException.h"
#include "domain/iam/role/Role.h"

using namespace drogon;

Task<Role> RoleAssembler::fromCreateRequest(const RoleCreateRequest request,
                                            std::int32_t createdBy)
{
    // 验证
    co_await roleVerifier_->verifyRoleNameNotDuplicated(request.name());
    co_await roleVerifier_->verifyRoleCodeNotDuplicated(request.code());
    if (request.relationType() != RelationType::All)
    {
        if (!request.deptIds() || request.deptIds()->size() == 0)
        {
            throw BusinessException("请选择关联部门");
        }
        co_await deptVerifier_->verifyDeptIdsExist(*request.deptIds());
    }

    // 必备参数
    Role role{request.name(),
              request.code(),
              request.quotaType(),
              request.relationType(),
              createdBy};

    // 可选参数
    role.constructOptionalFields(request.description(),
                                 request.userQuota(),
                                 request.deptIds(),
                                 createdBy);

    role.markNew();
    co_return role;
}
