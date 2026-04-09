#include "User.h"

using namespace std;
using namespace trantor;

User::User(const string &username,
           const string &nickname,
           const Sex &sex,
           const int32_t &deptId,
           const Status &status)
    : username_{username},
      nickname_{nickname},
      avatar_{"#"},
      sex_{sex},
      deptId_{deptId},
      status_{status}
{
}

User::User(const string &username,
           const string &nickname,
           const Sex &sex,
           const int32_t &deptId,
           const Status &status,
           const int32_t createdBy)
    : username_{username},
      nickname_{nickname},
      avatar_{"#"},
      sex_{sex},
      deptId_{deptId},
      status_{status},
      AuditableEntity{createdBy, Date::now(), createdBy, Date::now()}
{
}

User::User(const SysUser &model)
    : OPT_INIT(userId, UserId),
      INIT(username, Username),
      INIT(nickname, Nickname),
      INIT(avatar, Avatar),
      ENUM_INIT(Sex, sex, Sex),
      INIT(deptId, DeptId),
      OPT_INIT(phoneNumber, PhoneNumber),
      OPT_INIT(email, Email),
      ENUM_INIT(Status, status, Status),
      AuditableEntity(AUDITABLE_INIT)
{
}

User::operator SysUser() const
{
    SysUser model;
    if (userId_)
    {
        model.setUserId(*userId_);
    }
    model.setUsername(username_);
    model.setPassword("123456");
    model.setNickname(nickname_);
    model.setAvatar(avatar_);
    model.setSex(static_cast<int16_t>(sex_));
    model.setDeptId(deptId_);
    if (phoneNumber_)
    {
        model.setPhoneNumber(*phoneNumber_);
    }
    if (email_)
    {
        model.setEmail(*email_);
    }
    model.setStatus(static_cast<int16_t>(status_));
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

void User::addUserRole(const UserRole &userRole)
{
    this->userRoles_.push_back(userRole);
}

void User::addUserRoleByRoleId(const std::int32_t &roleId)
{
}
