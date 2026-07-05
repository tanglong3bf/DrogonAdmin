#pragma once

#include "domain/iam/module/ModuleRepository.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/utils/coroutine.h>

class ModuleVerifier : public DrAdminObject<ModuleVerifier>
{
  public:
    /**
     * @brief 校验模块没有子模块
     */
    drogon::Task<> verifyNoSubmodule(const int32_t moduleId) const;

  private:
    ModuleRepositoryPtr moduleRepository_{
        drogon::DrClassMap::getSingleInstance<ModuleRepository>()};
};

using ModuleVerifierPtr = std::shared_ptr<ModuleVerifier>;
