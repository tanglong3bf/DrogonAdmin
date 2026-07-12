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

    /**
     * @brief 排序模块
     */
    drogon::Task<std::vector<Module>> sortModule(
        const std::optional<std::int32_t> &parentId,
        const std::vector<int32_t> &deptIds,
        const int32_t updatedBy) const;

  protected:
    void validateModuleIdsInAllModules(
        const std::vector<std::int32_t> &moduleIds,
        const std::vector<Module> &allModules) const;

  private:
    ModuleVerifierPtr moduleVerifier_{
        drogon::DrClassMap::getSingleInstance<ModuleVerifier>()};
    RoleVerifierPtr roleVerifier_{
        drogon::DrClassMap::getSingleInstance<RoleVerifier>()};
    MenuVerifierPtr menuVerifier_{
        drogon::DrClassMap::getSingleInstance<MenuVerifier>()};
    ModuleRepositoryPtr moduleRepository_{
        drogon::DrClassMap::getSingleInstance<ModuleRepository>()};
};

using ModuleHandlerPtr = std::shared_ptr<ModuleHandler>;
