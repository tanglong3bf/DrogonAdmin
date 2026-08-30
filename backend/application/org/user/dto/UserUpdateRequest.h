#pragma once

#include "domain/org/user/Email.h"
#include "domain/org/user/PhoneNumber.h"
#include "domain/org/user/Sex.h"
#include "domain/org/user/Status.h"
#include "common/util/Utilities.hpp"
#include "common/util/ParamGetter.hpp"
#include <jsoncpp/json/value.h>
#include <optional>
#include <string>
#include <vector>

/**
 * @brief (后台)更新用户请求
 */
struct UserUpdateRequest
{
    UserUpdateRequest() = default;
    UserUpdateRequest(const Json::Value &json);

    GETTER(nickname)
    GETTER(sex)
    GETTER(deptId)
    GETTER(phoneNumber)
    GETTER(email)
    GETTER(status)
    GETTER(version)
    GETTER(roleIds)

  private:
    std::optional<std::string> nickname_;                         ///< 昵称
    std::optional<Sex> sex_;                                      ///< 性别
    std::optional<std::int32_t> deptId_;                          ///< 部门
    drogon_admin::util::NullableValue<PhoneNumber> phoneNumber_;  ///< 电话号码
    drogon_admin::util::NullableValue<Email> email_;              ///< 邮箱
    std::optional<Status> status_;                                ///< 状态
    std::int32_t version_;                              ///< 乐观锁版本号
    std::optional<std::vector<std::int32_t>> roleIds_;  ///< 新的角色id列表
};
