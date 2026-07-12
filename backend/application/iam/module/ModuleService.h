#pragma once

#include "ModuleResponse.h"
#include "ModuleCqrsRepo.h"
#include "ModuleCreateRequest.h"
#include "ModuleUpdateRequest.h"
#include "ModuleAssembler.h"
#include "ModuleUpdater.h"
#include "ModuleSortRequest.h"
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
     * @brief 创建模块
     */
    drogon::Task<> createModule(const ModuleCreateRequest &request,
                                const std::int32_t createdBy) const;

    /**
     * @brief 更新模块
     */
    drogon::Task<> updateModule(const std::int32_t deptId,
                                const ModuleUpdateRequest &request,
                                const std::int32_t updatedBy) const;

    /**
     * @brief 删除指定模块
     */
    drogon::Task<> deleteModule(const std::int32_t moduleId,
                                const std::int32_t deletedBy) const;

    /**
     * @brief 排序指定模块
     */
    drogon::Task<> sortModule(const ModuleSortRequest &request,
                              const std::int32_t updatedBy) const;

  private:
    ModuleCqrsRepoPtr moduleCqrsRepo_{
        drogon::DrClassMap::getSingleInstance<ModuleCqrsRepo>()};
    ModuleRepositoryPtr moduleRepository_{
        drogon::DrClassMap::getSingleInstance<ModuleRepository>()};
    ModuleHandlerPtr moduleHandler_{
        drogon::DrClassMap::getSingleInstance<ModuleHandler>()};
    ModuleAssemblerPtr moduleAssembler_{
        drogon::DrClassMap::getSingleInstance<ModuleAssembler>()};
    ModuleUpdaterPtr moduleUpdater_{
        drogon::DrClassMap::getSingleInstance<ModuleUpdater>()};
};

using ModuleServicePtr = std::shared_ptr<ModuleService>;
