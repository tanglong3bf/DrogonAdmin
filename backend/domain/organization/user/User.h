#pragma once

#include "Sex.h"
#include "Status.h"
#include "Email.h"
#include "PhoneNumber.h"
#include "UserRole.h"
#include "domain/models/SysUser.h"
#include "common/framework/domain/AuditableEntity.h"
#include "common/framework/domain/ChangeableEntity.h"
#include <optional>
#include <string>
#include <cstdint>

class User : public AuditableEntity, public ChangeableEntity
{
    using SysUser = drogon_model::drogon_admin_db::SysUser;

  public:
    explicit User(std::string username,
                  std::string nickname,
                  Sex sex,
                  int32_t deptId,
                  Status status);

    explicit User(std::string username,
                  std::string nickname,
                  Sex sex,
                  std::int32_t deptId,
                  Status status,
                  int32_t createdBy);
    // 与模型类互转
    explicit User(const SysUser &sysUser);
    explicit operator SysUser() const;

    std::string_view username() const noexcept
    {
        return username_;
    }

    void setUserRoles(const std::vector<UserRole> &userRoles);
    void addUserRole(const UserRole &userRole);

    std::optional<std::int32_t> userId;

  private:
    std::string username_;

  public:
    std::string nickname;
    std::string avatar;
    Sex sex;
    std::int32_t deptId;
    std::optional<PhoneNumber> phoneNumber;
    std::optional<Email> email;
    Status status;
    std::vector<UserRole> userRoles;
};
