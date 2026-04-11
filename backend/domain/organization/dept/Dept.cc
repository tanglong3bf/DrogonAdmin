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
    : OPT_INIT(deptId, DeptId),
      INIT(name, Name),
      INIT(sortNum, SortNum),
      OPT_INIT(parentId, ParentId),
      AuditableEntity{AUDITABLE_INIT_BY_MODEL}
{
}

Dept::operator SysDept() const
{
    SysDept model;
    SET_OPT(deptId, DeptId);
    SET_VAL(name, Name);
    SET_VAL(sortNum, SortNum);
    SET_OPT(parentId, ParentId);
    SET_OPT(createdBy, CreatedBy);
    SET_OPT(createdTime, CreatedTime);
    SET_OPT(updatedBy, UpdatedBy);
    SET_OPT(updatedTime, UpdatedTime);
    SET_OPT(deletedBy, DeletedBy);
    SET_OPT(deletedTime, DeletedTime);
    return model;
}
