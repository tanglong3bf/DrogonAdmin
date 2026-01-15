#include "Dept.h"

using namespace std;
using namespace trantor;
using namespace drogon_model::drogon_admin_db;

Dept::Dept(const string &name, const uint32_t order)
    : name_{name}, order_{order}
{
}

Dept::Dept(const string &name, const uint32_t order, const uint32_t createdBy)
    : name_{name},
      order_{order},
      AuditableEntity{createdBy, Date::now(), createdBy, Date::now()}
{
}

Dept::Dept(const SysDept &sysDept)
    : deptId_(sysDept.getValueOfDeptId()),
      name_(sysDept.getValueOfName()),
      order_(sysDept.getValueOfOrderNo()),
      parentId_(sysDept.getParentId() != nullptr
                    ? make_optional(sysDept.getValueOfParentId())
                    : nullopt),
      AuditableEntity(sysDept.getValueOfCreatedBy(),
                      sysDept.getValueOfCreatedTime(),
                      sysDept.getValueOfDeletedBy(),
                      sysDept.getValueOfUpdatedTime(),
                      sysDept.getDeletedBy() != nullptr
                          ? make_optional(sysDept.getValueOfDeletedBy())
                          : nullopt,
                      sysDept.getDeletedTime() != nullptr
                          ? make_optional(sysDept.getValueOfDeletedTime())
                          : nullopt)
{
}

Dept::operator drogon_model::drogon_admin_db::SysDept() const
{
    SysDept sysDept;
    if (deptId_)
    {
        sysDept.setDeptId(*deptId_);
    }
    sysDept.setName(name_);
    sysDept.setOrderNo(order_);
    if (parentId_)
    {
        sysDept.setParentId(*parentId_);
    }
    sysDept.setCreatedBy(*createdBy_);
    sysDept.setCreatedTime(*createdTime_);
    sysDept.setUpdatedBy(*updatedBy_);
    sysDept.setUpdatedTime(*updatedTime_);
    if (deletedBy_)
    {
        sysDept.setDeletedBy(*deletedBy_);
    }
    if (deletedTime_)
    {
        sysDept.setDeletedTime(*deletedTime_);
    }
    return sysDept;
}
