#pragma once

#include <drogon/utils/coroutine.h>
#include "domain/DeptHandler.h"
#include "DeptCqrsRepo.h"
#include "DeptAssembler.h"
#include "DeptCreateRequest.h"
#include "DeptUpdateRequest.h"
#include "DeptResponse.h"

class DeptService : public DrAdminObject<DeptService>
{
  public:
    DeptService() = default;

  public:
    /**
     * @brief 获取部门树
     */
    drogon::Task<std::vector<DeptResponse>> getDeptTree() const;

    /**
     * @brief 创建部门
     */
    drogon::Task<> createDept(const DeptCreateRequest &request,
                              const std::uint32_t createdBy);

    /**
     * @brief 更新部门名称
     */
    drogon::Task<> updateDept(const std::uint32_t deptId,
                              const DeptUpdateRequest &request,
                              const std::uint32_t updatedBy);

  private:
    DeptCqrsRepoPtr deptCqrsRepo_{
        drogon::DrClassMap::getSingleInstance<DeptCqrsRepo>()};
    DeptRepositoryPtr deptRepository_{
        drogon::DrClassMap::getSingleInstance<DeptRepository>()};
    DeptAssemblerPtr deptAssembler_{
        drogon::DrClassMap::getSingleInstance<DeptAssembler>()};
    DeptHandlerPtr deptHandler_{
        drogon::DrClassMap::getSingleInstance<DeptHandler>()};
};

using DeptServicePtr = std::shared_ptr<DeptService>;
