#include "ModuleUpdater.h"

#include "domain/iam/module/Action.h"
#include "domain/iam/module/ActionPriority.h"
#include "common/util/rangesUtils.hpp"
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <ranges>

using namespace std;
using namespace drogon;
using namespace drogon_admin;

Task<> ModuleUpdater::updateModule(Module &module,
                                   const ModuleUpdateRequest &request,
                                   const std::int32_t updatedBy)
{
    // 校验
    if (request.name() && request.name() != module.name())
    {
        co_await moduleVerifier_->verifyModuleNameNotDuplicated(
            *request.name(), module.parentId());
    }

    // 更新
    module.updateBasicInfo(request.name(), request.description(), updatedBy);

    co_return;
}

drogon::Task<> ModuleUpdater::updateActions(Module &module,
                                            const ActionUpdateRequest &request,
                                            const std::int32_t updatedBy)
{
    // 获取模块ID
    auto moduleId = module.moduleId();
    if (!moduleId.has_value())
    {
        // 模块必须已保存到数据库才能更新actions
        co_return;
    }

    // 存储ID到Action对象的映射
    std::unordered_map<std::int64_t, const Action *> newActionsMapping;
    const auto newActions = request.actions() |
                            views::transform([](const ActionRequest &a) {
                                return Action{a.actionId(),
                                              a.name(),
                                              a.code(),
                                              a.sortNum(),
                                              a.hasDataPermission(),
                                              a.moduleId()};
                            }) |
                            ranges_utils::to<vector>();
    for (const Action &a : newActions)
    {
        newActionsMapping[a.actionId()] = &a;
    };

    // 更新actions
    module.updateActions(newActionsMapping, updatedBy);

    // 更新priorities
    module.updatePriorities(
        request.priorities() |
            views::transform([moduleId](const PriorityRequest &p) {
                return ActionPriority{p.highId(), p.lowId(), *moduleId};
            }) |
            ranges_utils::to<vector>(),
        updatedBy);

    co_return;
}
