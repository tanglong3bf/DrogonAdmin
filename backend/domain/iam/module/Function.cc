#include "Function.h"

using namespace std;
using namespace trantor;

Function::Function(string_view name,
                   const string_view code,
                   const int32_t sortNum,
                   const int32_t moduleId)
    : name_{name}, code_{code}, sortNum_{sortNum}, moduleId_{moduleId}
{
}

Function::Function(string_view name,
                   const string_view code,
                   const int32_t sortNum,
                   const int32_t moduleId,
                   const int32_t createdBy)
    : name_{name},
      code_{code},
      sortNum_{sortNum},
      moduleId_{moduleId},
      AuditableEntity{AUDITABLE_INIT}
{
}

Function::Function(const SysFunction &model)
    : functionId_{model.getValueOfFunctionId()},
      name_{model.getValueOfName()},
      code_{model.getValueOfCode()},
      sortNum_{model.getValueOfSortNum()},
      moduleId_{model.getValueOfModuleId()},
      AuditableEntity{AUDITABLE_INIT_BY_MODEL}
{
}

Function::operator SysFunction() const
{
    SysFunction model;
    SET_OPT(functionId_, FunctionId);
    SET_VAL(name_, Name);
    SET_VAL(code_, Code);
    SET_OPT(description_, Description);
    SET_VAL(sortNum_, SortNum);
    SET_VAL(moduleId_, ModuleId);
    SET_OPT(createdBy(), CreatedBy);
    SET_OPT(createdTime(), CreatedTime);
    SET_OPT(updatedBy(), UpdatedBy);
    SET_OPT(updatedTime(), UpdatedTime);
    SET_OPT(deletedBy(), DeletedBy);
    SET_OPT(deletedTime(), DeletedTime);
    return model;
}
