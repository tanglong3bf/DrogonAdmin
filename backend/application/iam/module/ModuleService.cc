#include "ModuleService.h"

#include "common/util/rangesUtils.hpp"
#include <ranges>

using namespace std;
using namespace drogon;
using namespace drogon_admin;

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

Task<> ModuleService::deleteModule(const int32_t moduleId,
                                   const int32_t version,
                                   const int32_t deletedBy) const
{
    auto module = co_await moduleRepository_->getById(moduleId, true);
    if (!module)
    {
        // 没有数据，无需操作
        co_return;
    }
    co_await moduleHandler_->deleteModule(*module, version, deletedBy);
    co_await moduleRepository_->save(*module);
}

Task<> ModuleService::sortModule(const ModuleSortRequest &request,
                                 const int32_t updatedBy) const
{
    vector<Module> sortResult =
        co_await moduleHandler_->sortModule(request.parentId(),
                                            request.moduleIds(),
                                            updatedBy);

    if (sortResult.size() == 0)
    {
        co_return;
    }

    const vector<int32_t> versions =
        // 排序后结果
        sortResult |
        // 取id
        views::transform(
            [](const auto &module) { return *module.moduleId(); }) |
        // 从请求中找到version
        views::transform([&request](const auto &moduleId) {
            return ranges::find_if(request.modules(),
                                   [moduleId](const auto &item) {
                                       return moduleId == item.moduleId;
                                   });
        }) |
        views::transform([](const auto &it) { return it->version; }) |
        ranges_utils::to<vector>();

    for (size_t i = 0; i < sortResult.size(); i++)
    {
        if (versions[i] != sortResult[i].version())
        {
            throw BusinessException("更新期间数据发生变化，更新失败");
        }
    }

    co_await moduleRepository_->multiSave(sortResult, versions);
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
