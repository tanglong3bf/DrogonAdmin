#include "Dept.h"

using namespace std;
using namespace trantor;
using namespace drogon_model::drogon_admin_db;

Dept::Dept(const string &name, const int32_t sortNum)
    : name_{name}, sortNum_{sortNum}
{
}

Dept::Dept(const string &name, const int32_t sortNum, const int32_t createdBy)
    : name_{name}, sortNum_{sortNum}, AuditableEntity{AUDITABLE_INIT}
{
}

Dept::Dept(const SysDept &model)
    : OPT_INIT(deptId_, DeptId),
      INIT(name_, Name),
      INIT(sortNum_, SortNum),
      OPT_INIT(parentId_, ParentId),
      AuditableEntity{AUDITABLE_INIT_BY_MODEL}
{
}

Dept::operator SysDept() const
{
    SysDept model;
    SET_OPT(deptId_, DeptId);
    SET_VAL(name_, Name);
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

void Dept::setParentId(const std::optional<std::int32_t> parentId)
{
    if (parentId)
    {
        parentId_ = parentId;
    }
}

void Dept::updateName(std::string_view name, const int32_t updatedBy)
{
    name_ = name;
    markUpdatedBy(updatedBy);
    markUpdated();
}

void Dept::remove(const std::int32_t deletedBy)
{
    markDeletedBy(deletedBy);
    markDeleted();
}
