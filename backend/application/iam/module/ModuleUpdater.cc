#include "ModuleUpdater.h"

#include "domain/iam/module/Action.h"
#include "domain/iam/module/ActionPriority.h"
#include "common/util/rangesUtils.hpp"
#include <vector>
#include <unordered_map>
#include <ranges>
#include <cstdint>

using namespace std;
using namespace drogon;
using namespace drogon_admin;

Task<> ModuleUpdater::updateModule(Module &module,
                                   const ModuleUpdateRequest &request,
                                   const int32_t updatedBy)
{
    assert(module.moduleId() != nullopt);

    // 校验
    if (module.version() != request.version())
    {
        throw BusinessException{"更新期间数据发生变化，更新失败"};
    }
    if (request.name() && request.name() != module.name())
    {
        co_await moduleVerifier_->verifyModuleNameNotDuplicated(
            *request.name(), module.parentId());
    }

    // 更新
    module.updateBasicInfo(request.name(), request.description(), updatedBy);

    co_return;
}

Task<> ModuleUpdater::updateActions(Module &module,
                                    const ActionUpdateRequest &request,
                                    const int32_t updatedBy)
{
    assert(module.moduleId() != nullopt);

    // 校验
    if (module.version() != request.version())
    {
        throw BusinessException{"更新期间数据发生变化，更新失败"};
    }
    const vector<int32_t> actionIds =
        request.actions() | views::filter([](const ActionRequest &a) {
            return a.actionId() < INT32_MAX;
        }) |
        views::transform([](const ActionRequest &a) {
            return static_cast<int32_t>(a.actionId());
        }) |
        ranges_utils::to<vector>();

    if (actionIds.size() > 0)
    {
        co_await moduleVerifier_->verifyActionsBelongsToModule(
            *module.moduleId(), actionIds);
    }

    // 存储ID到Action对象的映射
    unordered_map<int64_t, const Action *> newActionsMapping;
    const auto newActions = request.actions() |
                            views::transform([&module](const ActionRequest &a) {
                                return Action{a.actionId(),
                                              a.name(),
                                              a.code(),
                                              a.sortNum(),
                                              a.hasDataPermission(),
                                              *module.moduleId()};
                            }) |
                            ranges_utils::to<vector>();
    for (const Action &a : newActions)
    {
        newActionsMapping[a.actionId()] = &a;
    };

    // 更新actions
    module.updateActions(newActionsMapping, updatedBy);

    // 更新priorities
    module.updatePriorities(request.priorities() |
                                views::transform([module](
                                                     const PriorityRequest &p) {
                                    return ActionPriority{p.highId(),
                                                          p.lowId(),
                                                          *module.moduleId()};
                                }) |
                                ranges_utils::to<vector>(),
                            updatedBy);

    co_return;
}
