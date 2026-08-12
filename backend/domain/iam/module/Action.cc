#include "Action.h"

using namespace std;
using namespace trantor;

Action::Action(const int64_t actionId,
               const string_view name,
               const string_view code,
               const int32_t sortNum,
               const bool hasDataPermission,
               const int32_t moduleId)
    : actionId_{actionId},
      name_{name},
      code_{code},
      sortNum_{sortNum},
      hasDataPermission_{hasDataPermission},
      moduleId_{moduleId}
{
}

Action::Action(const int64_t actionId,
               const string_view name,
               const string_view code,
               const int32_t sortNum,
               const bool hasDataPermission,
               const int32_t moduleId,
               const int32_t createdBy)
    : actionId_{actionId},
      name_{name},
      code_{code},
      sortNum_{sortNum},
      hasDataPermission_{hasDataPermission},
      moduleId_{moduleId},
      AuditableEntity{AUDITABLE_INIT}
{
    markNew();
}

Action::Action(const SysAction &model)
    : actionId_{model.getValueOfActionId()},
      name_{model.getValueOfName()},
      code_{model.getValueOfCode()},
      sortNum_{model.getValueOfSortNum()},
      hasDataPermission_{model.getValueOfHasDataPermission()},
      moduleId_{model.getValueOfModuleId()},
      AuditableEntity{AUDITABLE_INIT_BY_MODEL}
{
}

Action::operator SysAction() const
{
    SysAction model;
    SET_VAL(actionId_, ActionId);
    SET_VAL(name_, Name);
    SET_VAL(code_, Code);
    SET_OPT(description_, Description);
    SET_VAL(sortNum_, SortNum);
    SET_VAL(hasDataPermission_, HasDataPermission);
    SET_VAL(moduleId_, ModuleId);
    SET_OPT(createdBy(), CreatedBy);
    SET_OPT(createdTime(), CreatedTime);
    SET_OPT(updatedBy(), UpdatedBy);
    SET_OPT(updatedTime(), UpdatedTime);
    SET_OPT(deletedBy(), DeletedBy);
    SET_OPT(deletedTime(), DeletedTime);
    return model;
}

void Action::updateInfo(const string_view name,
                        const string_view code,
                        const int32_t sortNum,
                        const bool hasDataPermission,
                        const int32_t updatedBy)
{
    bool isUpdated = false;
    if (name_ != name)
    {
        name_ = name;
        isUpdated = true;
    }
    if (code_ != code)
    {
        code_ = code;
        isUpdated = true;
    }
    if (sortNum_ != sortNum)
    {
        sortNum_ = sortNum;
        isUpdated = true;
    }
    if (hasDataPermission_ != hasDataPermission)
    {
        hasDataPermission_ = hasDataPermission;
        isUpdated = true;
    }
    if (isUpdated)
    {
        markUpdatedBy(updatedBy);
        markUpdated();
    }
}
