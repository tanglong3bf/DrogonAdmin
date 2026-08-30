#pragma once

#include "dto/ModuleResponse.h"
#include "dto/ModuleCreateRequest.h"
#include "dto/ModuleUpdateRequest.h"
#include "dto/ModuleSortRequest.h"
#include "dto/ActionUpdateRequest.h"
#include "ModuleCqrsRepo.h"
#include "ModuleAssembler.h"
#include "ModuleUpdater.h"
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
                                std::int32_t createdBy) const;

    /**
     * @brief 更新模块
     */
    drogon::Task<> updateModule(std::int32_t deptId,
                                const ModuleUpdateRequest &request,
                                std::int32_t updatedBy) const;

    /**
     * @brief 删除指定模块
     */
    drogon::Task<> deleteModule(std::int32_t moduleId,
                                std::int32_t version,
                                std::int32_t deletedBy) const;

    /**
     * @brief 排序指定模块
     */
    drogon::Task<> sortModule(const ModuleSortRequest &request,
                              std::int32_t updatedBy) const;

    /**
     * @brief 更新模块
     */
    drogon::Task<> updateActions(std::int32_t moduleId,
                                 const ActionUpdateRequest &request,
                                 std::int32_t updatedBy) const;

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
