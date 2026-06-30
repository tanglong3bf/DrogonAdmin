#include "User.h"

#include "common/exception/BusinessException.h"
#include "common/util/Utilities.hpp"
#include "common/util/rangesUtils.hpp"
#include "common/util/third_party/BCryptCpp/BCrypt.h"
#include <unordered_set>
#include <ranges>

using namespace std;
using namespace trantor;
using namespace BCryptCpp;
using namespace drogon_admin;
using namespace drogon_admin::util;

bool matches(string_view raw, string_view mask)
{
    return BCrypt::CheckPassword(string(raw), string(mask));
}

User::User(string_view username,
           string_view password,
           string_view nickname,
           Sex sex,
           int32_t deptId,
           Status status)
    : username_{std::move(username)},
      password_{password},
      nickname_{std::move(nickname)},
      avatar_{"#"},
      sex_{sex},
      deptId_{deptId},
      status_{status}
{
}

User::User(std::string_view username,
           std::string_view password,
           std::string_view nickname,
           Sex sex,
           std::int32_t deptId,
           Status status,
           int32_t createdBy)
    : username_{std::move(username)},
      password_{password},
      nickname_{std::move(nickname)},
      avatar_{"#"},
      sex_{sex},
      deptId_{deptId},
      status_{status},
      AuditableEntity{AUDITABLE_INIT}
{
}

User::User(const SysUser &model)
    : OPT_INIT(userId_, UserId),
      INIT(username_, Username),
      INIT(password_, Password),
      INIT(nickname_, Nickname),
      INIT(avatar_, Avatar),
      ENUM_INIT(Sex, sex_, Sex),
      INIT(deptId_, DeptId),
      OPT_OV_INIT(PhoneNumber, phoneNumber_, PhoneNumber),
      OPT_OV_INIT(Email, email_, Email),
      ENUM_INIT(Status, status_, Status),
      AuditableEntity{AUDITABLE_INIT_BY_MODEL}
{
}

User::operator SysUser() const
{
    SysUser model;
    SET_OPT(userId_, UserId);
    SET_VAL(username_, Username);
    SET_VAL(password_, Password);
    SET_VAL(nickname_, Nickname);
    SET_VAL(avatar_, Avatar);
    SET_VAL_CAST(int16_t, sex_, Sex);
    SET_VAL(deptId_, DeptId);
    if (phoneNumber_)
    {
        model.setPhoneNumber(phoneNumber_->value());
    }
    else
    {
        model.setPhoneNumberToNull();
    }
    if (email_)
    {
        model.setEmail(email_->value());
    }
    else
    {
        model.setEmailToNull();
    }
    SET_VAL_CAST(int16_t, status_, Status);
    SET_OPT(createdBy(), CreatedBy);
    SET_OPT(createdTime(), CreatedTime);
    SET_OPT(updatedBy(), UpdatedBy);
    SET_OPT(updatedTime(), UpdatedTime);
    SET_OPT(deletedBy(), DeletedBy);
    SET_OPT(deletedTime(), DeletedTime);
    return model;
}

void User::constructOptionalFields(optional<PhoneNumber> phoneNumber,
                                   optional<Email> email)
{
    phoneNumber_ = phoneNumber;
    email_ = email;
}

void User::updatePassword(string_view oldPassword, string_view newPassword)
{
    if (!matches(oldPassword, password_))
    {
        throw BusinessException("旧密码不正确");
    }

    const auto salt = BCrypt::GenerateSalt();
    const auto hashedPassword =
        BCrypt::HashPassword(static_cast<string>(newPassword), salt);

    password_ = hashedPassword;
    markUpdatedBy(*userId_);
    markUpdated();
}

bool User::updateBasicInfo(optional<string_view> nickname,
                           optional<Sex> sex,
                           NullableValue<PhoneNumber> phoneNumber,
                           NullableValue<Email> email,
                           std::int32_t updatedBy)
{
    bool changed = false;

    ENTITY_SET(nickname, changed = true);
    ENTITY_SET(sex, changed = true);
    ENTITY_SET_OR_NULL(phoneNumber, changed = true);
    ENTITY_SET_OR_NULL(email, changed = true);

    if (changed)
    {
        markUpdatedBy(updatedBy > 0 ? updatedBy : *this->userId_);
        markUpdated();
    }

    return changed;
}

bool User::updateStatus(Status status, int32_t updatedBy)
{
    if (status != status_) [[likely]]
    {
        status_ = status;
        markUpdatedBy(updatedBy);
        markUpdated();
        return true;
    }
    return false;
}

bool User::assignToDept(std::int32_t deptId, std::int32_t updatedBy)
{
    if (deptId != deptId_) [[likely]]
    {
        deptId_ = deptId;
        markUpdatedBy(updatedBy);
        markUpdated();
        return true;
    }
    return false;
}

void User::remove(const std::int32_t deletedBy)
{
    if (changingStatus() == ChangingStatus::DELETED)
    {
        throw BusinessException("用户已删除");
    }
    markDeletedBy(deletedBy);
    markDeleted();
    for (auto &role : userRoles_)
    {
        role.markDeleted();
    }
}

void User::appendRoles(const std::vector<int32_t> &newRoleIds,
                       const int32_t createdBy)
{
    const auto newRoles =
        newRoleIds | views::transform([createdBy, this](const int32_t roleId) {
            UserRole ur{roleId, createdBy};
            ur.userId_ = userId_;
            ur.markNew();
            return ur;
        }) |
        ranges_utils::to<vector>();

    userRoles_.insert(userRoles_.end(), newRoles.begin(), newRoles.end());
}

void User::replaceRoles(const vector<int32_t> &roleIds, const int32_t updatedBy)
{
    unordered_set<int32_t> newRoleSet(roleIds.begin(), roleIds.end());

    // 标记删除
    for (auto &ur : userRoles_)
    {
        if (newRoleSet.find(ur.roleId()) == newRoleSet.end())
        {
            ur.markDeleted();
        }
    }

    // 准备已存在的角色id
    unordered_set<int32_t> existingRoleIds;
    for (const auto &rd : userRoles_)
    {
        existingRoleIds.insert(rd.roleId());
    }

    for (int32_t roleId : roleIds)
    {
        // 没有在已拥有的角色id列表寻找到
        if (existingRoleIds.find(roleId) == existingRoleIds.end())
        {
            // 新增
            UserRole newUR(roleId, updatedBy);
            newUR.userId_ = userId_;
            newUR.markNew();
            userRoles_.push_back(std::move(newUR));
        }
    }
}

void User::restoreRoles(const std::vector<SysUserRole> &sysUserRoles)
{
    userRoles_ =
        sysUserRoles |
        views::transform([](const SysUserRole &ur) { return UserRole{ur}; }) |
        ranges_utils::to<vector>();
}
