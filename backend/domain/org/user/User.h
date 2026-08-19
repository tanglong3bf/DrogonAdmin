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
#include <drogon/HttpRequest.h>
#include "domain/models/SysUserRole.h"
#include <optional>
#include <string>
#include <vector>
#include <cstdint>

/**
 * @brief 用户实体类
 *
 * @note 密码存储的是密文
 */
class User : public AuditableEntity, public ChangeableEntity
{
    using SysUser = drogon_model::drogon_admin_db::SysUser;
    using SysUserRole = drogon_model::drogon_admin_db::SysUserRole;

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

    void setAvatar(const std::string &avatarUrl);

    void constructOptionalFields(const std::optional<PhoneNumber> &phoneNumber,
                                 const std::optional<Email> &email);

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
        const drogon_admin::util::NullableValue<PhoneNumber> &phoneNumber,
        const drogon_admin::util::NullableValue<Email> &email,
        std::int32_t updatedBy = -1);

    bool updateStatus(Status status, std::int32_t updatedBy);

    bool assignToDept(std::int32_t deptId, std::int32_t updatedBy);

    void remove(std::int32_t deletedBy);

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
    GETTER(version)
    GETTER(userRoles)

    /**
     * @brief 追加角色（仅新增，不删除已有角色）
     * @param newRoleIds 待新增角色ID列表
     * @param createdBy 操作人ID，用于填充新建角色审计字段
     */
    void appendRoles(const std::vector<int32_t> &newRoleIds,
                     std::int32_t createdBy);

    /**
     * @brief 差量对齐更新角色：保留交集、删除不在新列表的旧角色、新增缺少角色
     * @param newRoleIds 最终需要持有的角色ID集合
     * @param updatedBy 本次更新操作人
     */
    void replaceRoles(const std::vector<int32_t> &roleIds, std::int32_t updatedBy);

    // 仓储重建聚合时调用
    void restoreRoles(const std::vector<SysUserRole> &sysUserRoles);

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
    std::int32_t version_;
    std::vector<UserRole> userRoles_;
};
