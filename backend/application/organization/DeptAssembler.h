#pragma once

#include <drogon/DrClassMap.h>
#include <drogon/utils/coroutine.h>
#include "common/framework/DrAdminObject.hpp"
#include "domain/organization/Dept.h"
#include "domain/organization/DeptRepository.h"
#include "domain/organization/DeptVerifier.h"
#include "DeptCreateRequest.h"

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
