#include "Module.h"

#include "common/exception/BusinessException.h"
#include "common/util/rangesUtils.hpp"
#include <ranges>

using namespace std;
using namespace trantor;
using namespace drogon_admin;

Module::Module(std::string_view name, const std::int32_t sortNum)
    : name_(name), sortNum_(sortNum)
{
}

Module::Module(std::string_view name,
               const std::int32_t sortNum,
               const std::int32_t createdBy)
    : name_(name), sortNum_(sortNum), AuditableEntity(AUDITABLE_INIT)
{
    markNew();
}

Module::Module(const SysModule &model)
    : OPT_INIT(moduleId_, ModuleId),
      INIT(name_, Name),
      OPT_INIT(description_, Description),
      INIT(sortNum_, SortNum),
      OPT_INIT(parentId_, ParentId),
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

void Module::restoreActions(const std::vector<SysAction> &sysActions)
{
    actions_ = sysActions |
               views::transform([](const SysAction &f) { return Action{f}; }) |
               ranges_utils::to<vector>();
}
