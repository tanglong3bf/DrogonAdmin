#pragma once

#include "ModuleUpdateRequest.h"
#include "domain/iam/module/Module.h"
#include "common/framework/DrAdminObject.hpp"
#include "domain/iam/module/ModuleVerifier.h"
#include <drogon/utils/coroutine.h>

class ModuleUpdater : public DrAdminObject<ModuleUpdater>
{
  public:
    /**
     * @brief 更新模块
     */
    drogon::Task<> updateModule(Module &module,
                                const ModuleUpdateRequest &request,
                                const std::int32_t updatedBy);

  private:
    ModuleVerifierPtr moduleVerifier_{
        drogon::DrClassMap::getSingleInstance<ModuleVerifier>()};
};

using ModuleUpdaterPtr = std::shared_ptr<ModuleUpdater>;
