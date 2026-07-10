#pragma once

#include "Module.h"
#include "ModuleVerifier.h"
#include "domain/iam/menu/MenuVerifier.h"
#include "domain/iam/role/RoleVerifier.h"
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
    RoleVerifierPtr roleVerifier_{
        drogon::DrClassMap::getSingleInstance<RoleVerifier>()};
    MenuVerifierPtr menuVerifier_{
        drogon::DrClassMap::getSingleInstance<MenuVerifier>()};
};

using ModuleHandlerPtr = std::shared_ptr<ModuleHandler>;
