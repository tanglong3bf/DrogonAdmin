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
      AuditableEntity{AUDITABLE_INIT}
{
}

User::User(const SysUser &model)
    : OPT_INIT(userId, UserId),
      INIT(username, Username),
      INIT(nickname, Nickname),
      INIT(avatar, Avatar),
      ENUM_INIT(Sex, sex, Sex),
      INIT(deptId, DeptId),
      OPT_OV_INIT(PhoneNumber, phoneNumber, PhoneNumber),
      OPT_OV_INIT(Email, email, Email),
      ENUM_INIT(Status, status, Status),
      AuditableEntity{AUDITABLE_INIT_BY_MODEL}
{
}

User::operator SysUser() const
{
    SysUser model;
    SET_OPT(userId, UserId);
    SET_VAL(username, Username);
    SET_VAL(nickname, Nickname);
    SET_VAL(avatar, Avatar);
    SET_VAL_CAST(int16_t, sex, Sex);
    SET_VAL(deptId, DeptId);
    SET_OPT_OV(phoneNumber, PhoneNumber);
    SET_OPT_OV(email, Email);
    SET_VAL_CAST(int16_t, status, Status);
    SET_OPT(createdBy, CreatedBy);
    SET_OPT(createdTime, CreatedTime);
    SET_OPT(updatedBy, UpdatedBy);
    SET_OPT(updatedTime, UpdatedTime);
    SET_OPT(deletedBy, DeletedBy);
    SET_OPT(deletedTime, DeletedTime);
    return model;
}

void User::addUserRole(const UserRole &userRole)
{
    this->userRoles_.push_back(userRole);
}
