#pragma once

#include <drogon/utils/coroutine.h>
#include <memory>
#include "GetRoleListRequest.h"
#include "RoleCqrsRepo.h"
#include "RoleResponse.h"
#include "application/authorization/RoleAssembler.h"
#include "application/authorization/RoleCreateRequest.h"
#include "common/framework/DrAdminObject.hpp"
#include "domain/authorization/RoleHandler.h"
#include "domain/authorization/RoleRepository.h"
#include "common/util/PaginatedResponse.hpp"

/**
 * @brief 角色服务
 */
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
     */
    drogon::Task<PaginatedResponse<RoleResponse>> getRoleList(
        const GetRoleListRequest &request) const;

    /**
     * @brief 创建角色
     */
    drogon::Task<> createRole(const RoleCreateRequest &request,
                              const std::int32_t createdBy) const;

    /**
     * @brief 删除角色
     */
    drogon::Task<> deleteRole(const std::int32_t roleId,
                              const std::int32_t deletedBy) const;

  private:
    RoleRepositoryPtr roleRepository_{
        drogon::DrClassMap::getSingleInstance<RoleRepository>()};
    RoleCqrsRepoPtr roleCqrsRepo_{
        drogon::DrClassMap::getSingleInstance<RoleCqrsRepo>()};
    RoleAssemblerPtr roleAssembler_{
        drogon::DrClassMap::getSingleInstance<RoleAssembler>()};
    RoleHandlerPtr roleHandler_{
        drogon::DrClassMap::getSingleInstance<RoleHandler>()};
};

using RoleServicePtr = std::shared_ptr<RoleService>;
