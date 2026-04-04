#include "User.h"

using namespace std;

User::User(const SysUser &model)
    : INIT(userId, UserId),
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
    model.setUserId(userId_);
    model.setUsername(username_);
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
