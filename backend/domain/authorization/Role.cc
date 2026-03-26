#include "Role.h"

using namespace std;

template <>
std::string toString(const QuotaType &quotaType)
{
    switch (quotaType)
    {
        case QuotaType::Unlimited:
            return "Unlimited";
        case QuotaType::TotalLimit:
            return "TotalLimit";
        case QuotaType::PerDeptLimit:
            return "PerDeptLimit";
    }
    LOG_WARN << "错误的 QuotaType :" << static_cast<int32_t>(quotaType);
    return "[error]";
}

Role::Role(const drogon_model::drogon_admin_db::SysRole &sysRole)
    : roleId_{sysRole.getValueOfRoleId()},
      name_{sysRole.getValueOfName()},
      code_{sysRole.getValueOfCode()},
      description_{sysRole.getDescription() != nullptr
                       ? make_optional(sysRole.getValueOfDescription())
                       : nullopt},
      userQuota_{sysRole.getRoleId() != nullptr
                     ? make_optional(sysRole.getValueOfUserQuota())
                     : nullopt},
      quotaType_{static_cast<QuotaType>(sysRole.getValueOfQuotaType())},
      AuditableEntity(sysRole.getValueOfCreatedBy(),
                      sysRole.getValueOfCreatedTime(),
                      sysRole.getValueOfUpdatedBy(),
                      sysRole.getValueOfUpdatedTime(),
                      sysRole.getDeletedBy() != nullptr
                          ? make_optional(sysRole.getValueOfDeletedBy())
                          : nullopt,
                      sysRole.getDeletedTime() != nullptr
                          ? make_optional(sysRole.getValueOfDeletedTime())
                          : nullopt)
{
}
