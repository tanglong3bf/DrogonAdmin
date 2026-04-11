#include "Role.h"

using namespace std;

Role::Role(const std::string &name,
           const std::string &code,
           const QuotaType quotaType,
           const RelationType relationType)
    : name_{name},
      code_{code},
      quotaType_{quotaType},
      relationType_{relationType}
{
}

Role::Role(const std::string &name,
           const std::string &code,
           const QuotaType quotaType,
           const RelationType relationType,
           const std::int32_t createdBy)
    : name_{name},
      code_{code},
      quotaType_{quotaType},
      relationType_{relationType},
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
    if (roleId_)
    {
        model.setRoleId(*roleId_);
    }
    model.setName(name_);
    model.setCode(code_);
    if (description_)
    {
        model.setDescription(*description_);
    }
    model.setQuotaType(static_cast<int16_t>(quotaType_));
    if (userQuota_)
    {
        model.setUserQuota(*userQuota_);
    }
    model.setRelationType(static_cast<int16_t>(relationType_));
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

void Role::addRoleDept(const RoleDept &dept)
{
    depts_.push_back(dept);
}

void Role::setRoleDepts(const std::vector<RoleDept> &depts)
{
    depts_ = depts;
}
