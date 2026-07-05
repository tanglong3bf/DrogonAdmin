#pragma once

#include "Module.h"
#include "ModuleVerifier.h"
#include "common/framework/DrAdminObject.hpp"

/**
 * @brief 模块操作
 */
class ModuleHandler : public DrAdminObject<ModuleHandler>
{
  public:
    /**
     * @brief 删除模块
     */
    drogon::Task<> deleteModule(Module &module, const int32_t deletedBy) const;

  private:
    ModuleVerifierPtr moduleVerifier_{
        drogon::DrClassMap::getSingleInstance<ModuleVerifier>()};
};

using ModuleHandlerPtr = std::shared_ptr<ModuleHandler>;
