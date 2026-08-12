#include "ModuleService.h"

using namespace std;
using namespace drogon;

Task<vector<ModuleResponse>> ModuleService::getModuleTree() const
{
    co_return co_await moduleCqrsRepo_->getModuleTree();
}

Task<> ModuleService::createModule(const ModuleCreateRequest &request,
                                   const std::int32_t createdBy) const
{
    auto module =
        co_await moduleAssembler_->fromCreateRequest(request, createdBy);
    co_await moduleRepository_->save(module);
}

Task<> ModuleService::updateModule(const std::int32_t deptId,
                                   const ModuleUpdateRequest &request,
                                   const std::int32_t updatedBy) const
{
    auto module = co_await moduleRepository_->getById(deptId);
    if (!module)
    {
        throw BusinessException{"指定的模块id不存在"};
    }
    co_await moduleUpdater_->updateModule(*module, request, updatedBy);
    co_await moduleRepository_->save(*module);
}

Task<> ModuleService::deleteModule(const std::int32_t moduleId,
                                   const std::int32_t deletedBy) const
{
    auto module = co_await moduleRepository_->getById(moduleId, true);
    if (!module)
    {
        // 没有数据，无需操作
        co_return;
    }
    co_await moduleHandler_->deleteModule(*module, deletedBy);
    co_await moduleRepository_->save(*module);
}

Task<> ModuleService::sortModule(const ModuleSortRequest &request,
                                 const std::int32_t updatedBy) const
{
    auto sortResult = co_await moduleHandler_->sortModule(request.parentId(),
                                                          request.moduleIds(),
                                                          updatedBy);

    co_await moduleRepository_->multiSave(sortResult);
}

Task<> ModuleService::updateActions(const std::int32_t moduleId,
                                    const ActionUpdateRequest &request,
                                    const std::int32_t updatedBy) const
{
    auto module = co_await moduleRepository_->getById(moduleId, true);
    if (!module)
    {
        throw BusinessException{"指定的模块id不存在"};
    }

    co_await moduleUpdater_->updateActions(*module, request, updatedBy);
    co_await moduleRepository_->save(*module);
}
