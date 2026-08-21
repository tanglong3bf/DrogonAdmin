#include "Module.h"

#include "common/exception/BusinessException.h"
#include "common/util/rangesUtils.hpp"
#include <ranges>
#include <unordered_map>

using namespace std;
using namespace trantor;
using namespace drogon_admin;

Module::Module(std::string_view name, const std::int32_t sortNum)
    : name_{name}, sortNum_{sortNum}, version_{0}
{
}

Module::Module(std::string_view name,
               const std::int32_t sortNum,
               const std::int32_t createdBy)
    : name_{name},
      sortNum_{sortNum},
      version_{0},
      AuditableEntity(AUDITABLE_INIT)
{
    markNew();
}

Module::Module(const SysModule &model)
    : OPT_INIT(moduleId_, ModuleId),
      INIT(name_, Name),
      OPT_INIT(description_, Description),
      INIT(sortNum_, SortNum),
      OPT_INIT(parentId_, ParentId),
      INIT(version_, Version),
      AuditableEntity(AUDITABLE_INIT_BY_MODEL)
{
}

Module::operator SysModule() const
{
    SysModule model;
    SET_OPT(moduleId_, ModuleId);
    SET_VAL(name_, Name);
    if (description_)
    {
        model.setDescription(*description_);
    }
    else
    {
        model.setDescriptionToNull();
    }
    SET_VAL(sortNum_, SortNum);
    SET_OPT(parentId_, ParentId);
    SET_VAL(version_, Version);
    SET_OPT(createdBy(), CreatedBy);
    SET_OPT(createdTime(), CreatedTime);
    SET_OPT(updatedBy(), UpdatedBy);
    SET_OPT(updatedTime(), UpdatedTime);
    SET_OPT(deletedBy(), DeletedBy);
    SET_OPT(deletedTime(), DeletedTime);
    return model;
}

void Module::remove(const int32_t deletedBy)
{
    for (Action &func : actions_)
    {
        func.markDeletedBy(deletedBy);
        func.markDeleted();
    }
    markDeletedBy(deletedBy);
    markDeleted();
}

void Module::updateBasicInfo(
    const std::optional<std::string> &name,
    const drogon_admin::util::NullableValue<std::string> &description,
    const std::int32_t updatedBy)
{
    bool isUpdated = false;
    if (name && name_ != *name)
    {
        name_ = *name;
        isUpdated = true;
    }
    if (description && description_ != *description)
    {
        description_ = *description;
        isUpdated = true;
    }
    else if (description.isNull() && description_)
    {
        description_ = nullopt;
        isUpdated = true;
    }
    if (isUpdated)
    {
        markUpdatedBy(updatedBy);
        markUpdated();
    }
}

void Module::appendActions(std::vector<Action> &actions,
                           const int32_t createdBy)
{
    // 仅有已存储到数据库中的模块可以添加功能
    assert(moduleId_.has_value());
    for (auto &func : actions)
    {
        func.moduleId_ = *moduleId_;
        func.createdBy_ = createdBy;
    }
    actions_.insert(actions_.end(), actions.begin(), actions.end());
}

void Module::replaceActions(const std::vector<Action> &newActions,
                            const int32_t updatedBy)
{
    throw BusinessException("函数未实现");
}

void Module::updateActions(
    const unordered_map<int64_t, const Action *> &newActionsMapping,
    const int32_t updatedBy)
{
    bool isAnyUpdated = false;
    for (auto &a : actions_)
    {
        if (newActionsMapping.find(a.actionId()) == newActionsMapping.end())
        {
            a.markDeleted();
            a.markDeletedBy(updatedBy);
        }
        else
        {
            bool isUpdated = false;
            auto newAction = newActionsMapping.at(a.actionId());

            if (a.name_ != newAction->name())
            {
                a.name_ = newAction->name();
                isUpdated = true;
            }
            if (a.code_ != newAction->code())
            {
                a.code_ = newAction->code();
                isUpdated = true;
            }
            if (a.hasDataPermission_ != newAction->hasDataPermission())
            {
                a.hasDataPermission_ = newAction->hasDataPermission();
                isUpdated = true;
            }
            if (a.sortNum_ != newAction->sortNum())
            {
                a.sortNum_ = newAction->sortNum();
                isUpdated = true;
            }

            if (isUpdated)
            {
                a.markUpdated();
                a.markUpdatedBy(updatedBy);
                isAnyUpdated = true;
            }
        }
    }

    auto newActions = newActionsMapping | views::filter([](const auto &a) {
                          return a.second->actionId() > INT32_MAX;
                      }) |
                      views::transform([](const auto &a) { return a.second; }) |
                      ranges_utils::to<vector>();
    for (const Action *a : newActions)
    {
        Action temp{a->actionId(),
                    a->name(),
                    a->code(),
                    a->sortNum(),
                    a->hasDataPermission(),
                    a->moduleId(),
                    updatedBy};
        actions_.push_back(temp);
    }

    if (newActions.size() > 0)
    {
        isAnyUpdated = true;
    }

    if (isAnyUpdated)
    {
        markUpdatedBy(updatedBy);
        markUpdated();
    }
}

void Module::updatePriorities(const std::vector<ActionPriority> &priorityList,
                              const int32_t updatedBy)
{
    bool isUpdated = false;
    for (auto &priority : priorities_)
    {
        std::pair<int64_t, int64_t> ids{priority.highId(), priority.lowId()};
        const auto iter =
            ranges::find_if(priorityList, [&ids](const ActionPriority &p) {
                return p.highId() == ids.first && p.lowId() == ids.second;
            });
        if (iter == priorityList.end())
        {
            priority.markDeleted();
            isUpdated = true;
        }
    }

    auto newPriorities =
        priorityList | views::filter([this](const auto &p) {
            return p.highId() > INT32_MAX || p.lowId() > INT32_MAX ||
                   ranges::find_if(priorities_, [&p](const auto &priority) {
                       return priority.highId() == p.highId() &&
                              priority.lowId() == p.lowId();
                   }) == priorities_.end();
        }) |
        ranges_utils::to<vector>();
    for (const ActionPriority p : newPriorities)
    {
        ActionPriority temp{p.highId(), p.lowId(), p.moduleId(), updatedBy};
        priorities_.push_back(temp);
    }

    if (isUpdated || newPriorities.size() > 0)
    {
        markUpdatedBy(updatedBy);
        markUpdated();
    }
}

void Module::restoreActions(const std::vector<SysAction> &sysActions)
{
    actions_ = sysActions |
               views::transform([](const SysAction &f) { return Action{f}; }) |
               ranges_utils::to<vector>();
}

void Module::restorePriorities(
    const std::vector<SysActionPriority> &sysActionPriorities)
{
    priorities_ = sysActionPriorities |
                  views::transform([](const SysActionPriority &f) {
                      return ActionPriority{f};
                  }) |
                  ranges_utils::to<vector>();
}
