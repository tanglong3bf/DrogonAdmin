#pragma once

#include <drogon/utils/coroutine.h>
#include "application/organization/dept/DeptSortRequest.h"
#include "domain/organization/dept/DeptHandler.h"
#include "application/authorization/role/RoleService.h"
#include "DeptCqrsRepo.h"
#include "DeptAssembler.h"
#include "DeptCreateRequest.h"
#include "DeptUpdateRequest.h"
#include "DeptResponse.h"

class DeptService : public DrAdminObject<DeptService>
{
  public:
    DeptService() = default;

    /**
     * @brief 获取部门树
     */
    drogon::Task<std::vector<DeptResponse>> getDeptTree() const;

    /**
     * @brief 创建部门
     */
    drogon::Task<> createDept(const DeptCreateRequest &request,
                              const std::int32_t createdBy) const;

    /**
     * @brief 更新部门名称
     */
    drogon::Task<> updateDept(const std::int32_t deptId,
                              const DeptUpdateRequest &request,
                              const std::int32_t updatedBy) const;

    /**
     * @brief 删除指定部门
     */
    drogon::Task<> deleteDept(const std::int32_t deptId,
                              const std::int32_t deletedBy) const;

    /**
     * @brief 排序指定部门
     */
    drogon::Task<> sortDept(const DeptSortRequest &request,
                            const std::int32_t updatedBy) const;

  private:
    DeptCqrsRepoPtr deptCqrsRepo_{
        drogon::DrClassMap::getSingleInstance<DeptCqrsRepo>()};
    DeptRepositoryPtr deptRepository_{
        drogon::DrClassMap::getSingleInstance<DeptRepository>()};
    DeptAssemblerPtr deptAssembler_{
        drogon::DrClassMap::getSingleInstance<DeptAssembler>()};
    DeptHandlerPtr deptHandler_{
        drogon::DrClassMap::getSingleInstance<DeptHandler>()};
    // 耦合
    RoleServicePtr roleService_{
        drogon::DrClassMap::getSingleInstance<RoleService>()};
};

using DeptServicePtr = std::shared_ptr<DeptService>;
