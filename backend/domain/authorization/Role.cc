#include "Role.h"

using namespace std;

Role::Role(const std::string &name,
           const std::string &code,
           const QuotaType quotaType,
           const RelationType relationType)
    : name{name}, code{code}, quotaType{quotaType}, relationType{relationType}
{
}

Role::Role(const std::string &name,
           const std::string &code,
           const QuotaType quotaType,
           const RelationType relationType,
           const std::int32_t createdBy)
    : name{name},
      code{code},
      quotaType{quotaType},
      relationType{relationType},
      AuditableEntity{createdBy,
                      trantor::Date::now(),
                      createdBy,
                      trantor::Date::now()}
{
}

Role::Role(const drogon_model::drogon_admin_db::SysRole &model)
    : INIT(roleId, RoleId),
      INIT(name, Name),
      INIT(code, Code),
      OPT_INIT(description, Description),
      ENUM_INIT(QuotaType, quotaType, QuotaType),
      OPT_INIT(userQuota, UserQuota),
      ENUM_INIT(RelationType, relationType, RelationType),
      AuditableEntity(AUDITABLE_INIT_BY_MODEL)
{
}

Role::operator SysRole() const
{
    SysRole model;
    if (roleId)
    {
        model.setRoleId(*roleId);
    }
    model.setName(name);
    model.setCode(code);
    if (description)
    {
        model.setDescription(*description);
    }
    model.setQuotaType(static_cast<int16_t>(quotaType));
    if (userQuota)
    {
        model.setUserQuota(*userQuota);
    }
    model.setRelationType(static_cast<int16_t>(relationType));
    model.setCreatedBy(*createdBy_);
    model.setCreatedTime(*createdTime_);
    model.setUpdatedBy(*updatedBy_);
    model.setUpdatedTime(*updatedTime_);
    if (deletedBy_)
    {
        model.setDeletedBy(*deletedBy_);
    }
    if (deletedTime_)
    {
        model.setDeletedTime(*deletedTime_);
    }
    return model;
}

void Role::setRoleDepts(const std::vector<RoleDept> &roleDepts)
{
    this->roleDepts.clear();
    std::copy(roleDepts.begin(),
              roleDepts.end(),
              std::back_inserter(this->roleDepts));
}

void Role::addRoleDept(const RoleDept &dept)
{
    roleDepts.push_back(dept);
}
