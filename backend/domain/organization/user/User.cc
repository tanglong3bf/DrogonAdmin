#include "User.h"

#include "common/exception/BusinessException.h"
#include <drogon/HttpAppFramework.h>
#include <drogon/MultiPart.h>
#include <drogon/HttpResponse.h>

using namespace std;
using namespace drogon;
using namespace trantor;

User::User(string_view username,
           string_view password,
           string_view nickname,
           Sex sex,
           int32_t deptId,
           Status status)
    : username_{std::move(username)},
      password{password},
      nickname{std::move(nickname)},
      avatar{"#"},
      sex{sex},
      deptId{deptId},
      status{status}
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
      password{password},
      nickname{std::move(nickname)},
      avatar{"#"},
      sex{sex},
      deptId{deptId},
      status{status},
      AuditableEntity{AUDITABLE_INIT}
{
}

User::User(const SysUser &model)
    : OPT_INIT(userId, UserId),
      INIT(username_, Username),
      INIT(password, Password),
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
    SET_VAL(username_, Username);
    SET_VAL(password, Password);
    SET_VAL(nickname, Nickname);
    SET_VAL(avatar, Avatar);
    SET_VAL_CAST(int16_t, sex, Sex);
    SET_VAL(deptId, DeptId);
    if (phoneNumber)
    {
        model.setPhoneNumber(phoneNumber->value());
    }
    else
    {
        model.setPhoneNumberToNull();
    }
    if (email)
    {
        model.setEmail(email->value());
    }
    else
    {
        model.setEmailToNull();
    }
    SET_VAL_CAST(int16_t, status, Status);
    SET_OPT(createdBy(), CreatedBy);
    SET_OPT(createdTime(), CreatedTime);
    SET_OPT(updatedBy(), UpdatedBy);
    SET_OPT(updatedTime(), UpdatedTime);
    SET_OPT(deletedBy(), DeletedBy);
    SET_OPT(deletedTime(), DeletedTime);
    return model;
}

Task<string> User::updateAvatar(const HttpRequestPtr &req)
{
    MultiPartParser parser;
    parser.parse(req);
    const auto files = parser.getFilesMap();
    if (!files.contains("avatar"))
    {
        throw BusinessException("缺少头像文件");
    }

    const HttpFile &avatarFile = files.at("avatar");
    const string_view extension = avatarFile.getFileExtension();
    const string fileName =
        avatarFile.getMd5() + "." + static_cast<string>(extension);
    avatarFile.saveAs(fileName);

    const auto imgPrefix =
        app().getCustomConfig().get("img_prefix", "/uploads/").asString();
    avatar = imgPrefix + fileName;

    this->markUpdated();
    this->markUpdatedBy(*this->userId);

    co_return avatar;
}

void User::setUserRoles(const std::vector<UserRole> &userRoles)
{
    this->userRoles.clear();
    std::copy(userRoles.begin(),
              userRoles.end(),
              std::back_inserter(this->userRoles));
}

void User::addUserRole(const UserRole &userRole)
{
    this->userRoles.push_back(userRole);
}
