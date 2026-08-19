#pragma once

#include "domain/org/user/Email.h"
#include "domain/org/user/PhoneNumber.h"
#include "domain/org/user/Sex.h"
#include "common/util/Utilities.hpp"
#include "common/util/ParamGetter.hpp"
#include <jsoncpp/json/value.h>
#include <optional>

/**
 * @brief (个人中心)更新用户请求
 */
struct UserInfoUpdateRequest
{
    UserInfoUpdateRequest() = default;
    UserInfoUpdateRequest(const Json::Value &json);

    GETTER(nickname)
    GETTER(sex)
    GETTER(phoneNumber)
    GETTER(email)
    GETTER(version)

  private:
    std::optional<std::string> nickname_;                         ///< 昵称
    std::optional<Sex> sex_;                                      ///< 性别
    drogon_admin::util::NullableValue<PhoneNumber> phoneNumber_;  ///< 电话号码
    drogon_admin::util::NullableValue<Email> email_;              ///< 邮箱
    std::int32_t version_;  ///< 乐观锁版本号
};
