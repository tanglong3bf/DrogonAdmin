#pragma once

#include "domain/organization/user/Email.h"
#include "domain/organization/user/PhoneNumber.h"
#include "domain/organization/user/Sex.h"
#include "domain/organization/user/Status.h"
#include "common/util/Utilities.hpp"
#include <jsoncpp/json/value.h>
#include <optional>
#include <string>
#include <vector>

/**
 * 用户更新请求
 */
class UserUpdateRequest
{
    std::optional<std::string> nickname_;               ///< 昵称
    std::optional<Sex> sex_;                            ///< 性别
    std::optional<std::int32_t> deptId_;                ///< 部门
    std::optional<PhoneNumber> phoneNumber_;            ///< 电话号码
    std::optional<Email> email_;                        ///< 邮箱
    std::optional<Status> status_;                      ///< 状态
    std::optional<std::vector<std::int32_t>> roleIds_;  ///< 新的角色id列表

  public:
    void setByJson(const Json::Value &json);

    OPT_GETTER(nickname, Nickname)
    OPT_GETTER(sex, Sex)
    OPT_GETTER(deptId, DeptId)
    OPT_GETTER(phoneNumber, PhoneNumber)
    OPT_GETTER(email, Email)
    OPT_GETTER(status, Status)
    GETTER(roleIds, RoleIds)
};
