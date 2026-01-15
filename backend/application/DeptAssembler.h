#pragma once

#include <drogon/utils/coroutine.h>
#include <drogon/DrClassMap.h>
#include "common/framework/DrAdminObject.hpp"
#include "domain/Dept.h"
#include "domain/DeptRepository.h"
#include "domain/DeptVerifier.h"
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
                                         std::uint32_t createdBy);

  private:
    DeptRepositoryPtr deptRepository_{
        drogon::DrClassMap::getSingleInstance<DeptRepository>()};
    DeptVerifierPtr deptVerifier_{
        drogon::DrClassMap::getSingleInstance<DeptVerifier>()};
};

using DeptAssemblerPtr = std::shared_ptr<DeptAssembler>;
