#pragma once

#include "Email.h"
#include "PhoneNumber.h"
#include "Sex.h"
#include "Status.h"
#include "UserRole.h"
#include "domain/models/SysUser.h"
#include "common/util/ParamGetter.hpp"
#include "common/framework/domain/AuditableEntity.h"
#include "common/framework/domain/ChangeableEntity.h"
#include <optional>
#include <string>
#include <cstdint>

class User : public AuditableEntity, public ChangeableEntity
{
    using SysUser = drogon_model::drogon_admin_db::SysUser;

  public:
    explicit User(std::string_view username,
                  std::string_view password,
                  std::string_view nickname,
                  Sex sex,
                  int32_t deptId,
                  Status status);

    explicit User(std::string_view username,
                  std::string_view password,
                  std::string_view nickname,
                  Sex sex,
                  std::int32_t deptId,
                  Status status,
                  int32_t createdBy);
    // 与模型类互转
    explicit User(const SysUser &sysUser);
    explicit operator SysUser() const;

    void constructOptionalFields(std::optional<PhoneNumber> phoneNumber,
                                 std::optional<Email> email);

    /**
     * @brief 更新密码
     */
    void updatePassword(std::string_view oldPassword,
                        std::string_view newPassword);

    /**
     * @brief 个人中心更新基本信息
     *
     * @param updatedBy 更新者id，非正值表示自己更新
     */
    bool updateBasicInfo(
        std::optional<std::string_view> nickname,
        std::optional<Sex> sex,
        drogon_admin::util::NullableValue<PhoneNumber> phoneNumber,
        drogon_admin::util::NullableValue<Email> email,
        std::int32_t updatedBy = -1);

    bool updateStatus(Status status, int32_t updatedBy);

    bool assignToDept(std::int32_t deptId, std::int32_t updatedBy);

    void updateUserRoles(const std::vector<int32_t> &newRoleIds,
                         const int32_t updatedBy);

    GETTER(userId)
    GETTER_STR_VIEW(username)
    GETTER_STR_VIEW(password)
    GETTER_STR_VIEW(nickname)
    GETTER_STR_VIEW(avatar)
    GETTER(sex)
    GETTER(deptId)
    GETTER(phoneNumber)
    GETTER(email)
    GETTER(status)
    GETTER(userRoles)

    void setUserRoles(const std::vector<UserRole> &userRoles);
    void addUserRole(const UserRole &userRole);

  private:
    std::optional<std::int32_t> userId_;
    std::string username_;
    std::string password_;
    std::string nickname_;
    std::string avatar_;
    Sex sex_;
    std::int32_t deptId_;
    std::optional<PhoneNumber> phoneNumber_;
    std::optional<Email> email_;
    Status status_;
    std::vector<UserRole> userRoles_;
};
