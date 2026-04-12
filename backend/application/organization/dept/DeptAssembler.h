#pragma once

#include "DeptCreateRequest.h"
#include "domain/organization/dept/Dept.h"
#include "domain/organization/dept/DeptRepository.h"
#include "domain/organization/dept/DeptVerifier.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/DrClassMap.h>
#include <drogon/utils/coroutine.h>

/**
 * 部门装配器
 */
class DeptAssembler : public DrAdminObject<DeptAssembler>
{
  public:
    /**
     * @brief 用DeptCreateRequest构建Dept对象
     */
    drogon::Task<Dept> fromCreateRequest(DeptCreateRequest request,
                                         std::int32_t createdBy);

  private:
    DeptRepositoryPtr deptRepository_{
        drogon::DrClassMap::getSingleInstance<DeptRepository>()};
    DeptVerifierPtr deptVerifier_{
        drogon::DrClassMap::getSingleInstance<DeptVerifier>()};
};

using DeptAssemblerPtr = std::shared_ptr<DeptAssembler>;
