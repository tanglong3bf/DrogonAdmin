#include "ActionPriority.h"

using namespace trantor;

ActionPriority::ActionPriority(const std::int64_t highId,
                               const std::int64_t lowId,
                               const std::int32_t moduleId)
    : highId_{highId}, lowId_{lowId}, moduleId_{moduleId}
{
}

ActionPriority::ActionPriority(const std::int64_t highId,
                               const std::int64_t lowId,
                               const std::int32_t moduleId,
                               const int32_t createdBy)
    : highId_{highId},
      lowId_{lowId},
      moduleId_{moduleId},
      createdBy_{createdBy},
      createdTime_{Date::now()}
{
    markNew();
}

ActionPriority::ActionPriority(const SysActionPriority &sysActionPriority)
    : highId_{sysActionPriority.getValueOfHighId()},
      lowId_{sysActionPriority.getValueOfLowId()},
      moduleId_{sysActionPriority.getValueOfModuleId()},
      createdBy_{sysActionPriority.getValueOfCreatedBy()},
      createdTime_{sysActionPriority.getValueOfCreatedTime()}
{
}

ActionPriority::operator SysActionPriority() const
{
    SysActionPriority model;
    SET_VAL(highId_, HighId);
    SET_VAL(lowId_, LowId);
    SET_VAL(moduleId_, ModuleId);
    SET_VAL(createdBy_, CreatedBy);
    SET_VAL(createdTime_, CreatedTime);
    return model;
}
