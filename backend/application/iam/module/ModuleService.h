#pragma once

#include "ModuleResponse.h"
#include "ModuleCqrsRepo.h"
#include "domain/iam/module/ModuleHandler.h"
#include "domain/iam/module/ModuleRepository.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/utils/coroutine.h>

/**
 * @brief 模块服务
 */
class ModuleService : public DrAdminObject<ModuleService>
{
  public:
    /**
     * @brief 获取模块树
     */
    drogon::Task<std::vector<ModuleResponse>> getModuleTree() const;

    /**
     * @brief 删除指定模块
     */
    drogon::Task<> deleteModule(const std::int32_t moduleId,
                                const std::int32_t deletedBy) const;

  private:
    ModuleCqrsRepoPtr moduleCqrsRepo_{
        drogon::DrClassMap::getSingleInstance<ModuleCqrsRepo>()};
    ModuleRepositoryPtr moduleRepository_{
        drogon::DrClassMap::getSingleInstance<ModuleRepository>()};
    ModuleHandlerPtr moduleHandler_{
        drogon::DrClassMap::getSingleInstance<ModuleHandler>()};
};

using ModuleServicePtr = std::shared_ptr<ModuleService>;
