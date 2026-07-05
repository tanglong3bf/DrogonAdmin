#include "Module.h"

using namespace trantor;

Module::Module(std::string_view name, const std::int32_t sortNum)
    : name_(name), sortNum_(sortNum)
{
}

Module::Module(std::string_view name,
               const std::int32_t sortNum,
               const std::int32_t createdBy)
    : name_(name), sortNum_(sortNum), AuditableEntity(AUDITABLE_INIT)
{
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
    SET_OPT(description_, Description);
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
    markDeletedBy(deletedBy);
    markDeleted();
}
