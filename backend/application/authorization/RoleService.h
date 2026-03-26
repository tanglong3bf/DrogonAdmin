#pragma once

#include <drogon/utils/coroutine.h>
#include <memory>
#include "GetRoleListRequest.h"
#include "RoleCqrsRepo.h"
#include "RoleResponse.h"
#include "common/framework/DrAdminObject.hpp"
#include "domain/authorization/RoleRepository.h"
#include "common/util/PaginatedResponse.hpp"

class RoleService : public DrAdminObject<RoleService>
{
  public:
    /**
     * @brief 删除部门时，删除排除该部门的角色关联数据
     */
    drogon::Task<> deleteExcludingDept(const std::int32_t deptId,
                                       const std::int32_t deletedBy) const;

    /**
     * @brief 带条件分页查询角色列表
     *
     * request.page可能会被修改
     */
    drogon::Task<PaginatedResponse<RoleResponse>> getRoleList(
        const GetRoleListRequest &request) const;

  private:
    RoleRepositoryPtr roleRepository_{
        drogon::DrClassMap::getSingleInstance<RoleRepository>()};
    RoleCqrsRepoPtr roleCqrsRepo_{
        drogon::DrClassMap::getSingleInstance<RoleCqrsRepo>()};
};

using RoleServicePtr = std::shared_ptr<RoleService>;
