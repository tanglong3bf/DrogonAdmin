#pragma once

#include "RoleAssembler.h"
#include "RoleCqrsRepo.h"
#include "RoleUpdater.h"
#include "dto/RoleQueryRequest.h"
#include "dto/RoleResponse.h"
#include "dto/AssignableRoleResponse.h"
#include "dto/RoleCreateRequest.h"
#include "dto/RoleUpdateRequest.h"
#include "domain/authorization/RoleHandler.h"
#include "domain/authorization/RoleRepository.h"
#include "common/framework/DrAdminObject.hpp"
#include "common/util/PaginatedResponse.hpp"
#include <drogon/utils/coroutine.h>
#include <memory>

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
        const RoleQueryRequest &request) const;

    /**
     * @brief 创建角色
     */
    drogon::Task<> createRole(const RoleCreateRequest &request,
                              const std::int32_t createdBy) const;

    /**
     * @brief 更新角色
     */
    drogon::Task<> updateRole(const std::int32_t roleId,
                              const RoleUpdateRequest request,
                              const std::int32_t updatedBy) const;

    /**
     * @brief 删除角色
     */
    drogon::Task<> deleteRole(const std::int32_t roleId,
                              const std::int32_t deletedBy) const;

    /**
     * @brief 获取指定部门可以分配的角色
     */
    drogon::Task<std::vector<AssignableRoleResponse>> getAssignableRoles(
        const std::int32_t deptId) const;

  private:
    RoleRepositoryPtr roleRepository_{
        drogon::DrClassMap::getSingleInstance<RoleRepository>()};
    RoleCqrsRepoPtr roleCqrsRepo_{
        drogon::DrClassMap::getSingleInstance<RoleCqrsRepo>()};
    RoleAssemblerPtr roleAssembler_{
        drogon::DrClassMap::getSingleInstance<RoleAssembler>()};
    RoleHandlerPtr roleHandler_{
        drogon::DrClassMap::getSingleInstance<RoleHandler>()};
    RoleUpdaterPtr roleUpdater_{
        drogon::DrClassMap::getSingleInstance<RoleUpdater>()};
};

using RoleServicePtr = std::shared_ptr<RoleService>;
