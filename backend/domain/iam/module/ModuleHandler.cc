#include "ModuleHandler.h"

#include "common/util/rangesUtils.hpp"
#include <ranges>

using namespace std;
using namespace drogon;
using namespace drogon_admin;

Task<> ModuleHandler::deleteModule(Module &module,
                                   const int32_t deletedBy) const
{
    // 校验
    // 检查没有子模块
    co_await moduleVerifier_->verifyNoSubmodule(*module.moduleId());

    const vector<std::int32_t> functionIds =
        module.functions() |
        views::transform([](const Function &f) { return *f.functionId(); }) |
        ranges_utils::to<vector>();

    if (functionIds.size() > 0)
    {
        // 检查模块的功能没有被分配权限
        co_await roleVerifier_->verifyFunctionNotUsed(functionIds);
        // 检查模块的功能没有被菜单使用
        co_await menuVerifier_->verifyFunctionNotUsed(functionIds);
    }

    // 标记删除
    module.remove(deletedBy);
}

Task<vector<Module>> ModuleHandler::sortModule(
    const optional<std::int32_t> &parentId,
    const vector<int32_t> &moduleIds,
    const int32_t updatedBy) const
{
    const auto allModules = co_await moduleRepository_->getByParentId(parentId);
    if (parentId)
    {
        const auto parent = co_await moduleRepository_->getById(*parentId);
        if (!parent)
        {
            throw BusinessException{"父模块不存在"};
        }
    }
    // 验证
    validateModuleIdsInAllModules(moduleIds, allModules);

    // 模块id到allModules索引的映射
    unordered_map<int32_t, size_t> idToIndex;
    for (size_t i = 0; i < allModules.size(); ++i)
    {
        auto id = *allModules[i].moduleId();
        idToIndex[id] = i;
    }

    // 哪些索引的模块已经被处理
    vector<bool> processed(allModules.size(), false);

    // 排序结果
    vector<Module> sortResult;
    sortResult.reserve(allModules.size());

    size_t moduleProcessedCount = 0;

    // 先处理dpetIds指定的模块
    for (int32_t id : moduleIds)
    {
        const auto it = idToIndex.find(id);
        if (it != idToIndex.end())
        {
            const int32_t newSortNum = moduleProcessedCount++;
            const size_t idx = it->second;
            if (newSortNum != allModules[idx].sortNum())
            {
                Module module = allModules[idx];
                module.updateSortNum(newSortNum, updatedBy);
                sortResult.push_back(std::move(module));
            }
            processed[idx] = true;
        }
    }

    // 再处理剩下的模块，保持原有顺序
    for (size_t i = 0; i < allModules.size(); ++i)
    {
        if (!processed[i])
        {
            const int32_t newSortNum = moduleProcessedCount++;

            if (newSortNum != allModules[i].sortNum())
            {
                Module module = allModules[i];
                module.updateSortNum(newSortNum, updatedBy);
                sortResult.push_back(std::move(module));
            }
        }
    }
    co_return sortResult;
}

void ModuleHandler::validateModuleIdsInAllModules(
    const vector<int32_t> &moduleIds,
    const vector<Module> &allModules) const
{
    unordered_set<int32_t> allModuleIds;
    for (const auto &module : allModules)
    {
        allModuleIds.insert(*module.moduleId());
    }

    for (const auto &moduleId : moduleIds)
    {
        if (allModuleIds.count(moduleId) == 0)
        {
            throw BusinessException{"部分模块不是指定模块的子模块"};
        }
    }
}
