#pragma once

#include "dto/ModuleUpdateRequest.h"
#include "dto/ActionUpdateRequest.h"
#include "domain/iam/module/Module.h"
#include "domain/iam/module/ModuleVerifier.h"
#include "common/framework/DrAdminObject.hpp"
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

    /**
     * @brief 更新功能
     */
    drogon::Task<> updateActions(Module &module,
                                 const ActionUpdateRequest &request,
                                 std::int32_t updatedBy);

  private:
    ModuleVerifierPtr moduleVerifier_{
        drogon::DrClassMap::getSingleInstance<ModuleVerifier>()};
};

using ModuleUpdaterPtr = std::shared_ptr<ModuleUpdater>;
