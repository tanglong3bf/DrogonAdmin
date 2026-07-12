#pragma once

#include "ModuleCreateRequest.h"
#include "domain/iam/module/Module.h"
#include "common/framework/DrAdminObject.hpp"
#include "domain/iam/module/ModuleRepository.h"
#include "domain/iam/module/ModuleVerifier.h"
#include <drogon/DrClassMap.h>
#include <drogon/utils/coroutine.h>

/**
 * 部门装配器
 */
class ModuleAssembler : public DrAdminObject<ModuleAssembler>
{
  public:
    /**
     * @brief 用ModuleCreateRequest构建Module对象
     */
    drogon::Task<Module> fromCreateRequest(ModuleCreateRequest request,
                                           std::int32_t createdBy);

  private:
    ModuleRepositoryPtr moduleRepository_{
        drogon::DrClassMap::getSingleInstance<ModuleRepository>()};
    ModuleVerifierPtr moduleVerifier_{
        drogon::DrClassMap::getSingleInstance<ModuleVerifier>()};
};

using ModuleAssemblerPtr = std::shared_ptr<ModuleAssembler>;
