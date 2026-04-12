#pragma once

#include "domain/organization/user/Sex.h"
#include "domain/organization/user/Status.h"
#include "common/util/Utilities.hpp"
#include <optional>
#include <string>
#include <cstdint>

/**
 * 用户查询请求
 */
class UserQueryRequest
{
    std::optional<std::string> username_;     ///< 用户名
    std::optional<std::string> nickname_;     ///< nicheng
    std::optional<Sex> sex_;                  ///< 性别
    std::optional<std::int32_t> deptId_;      ///< 部门
    std::optional<std::string> phoneNumber_;  ///< 手机号
    std::optional<std::string> email_;        ///< 邮箱
    std::optional<Status> status_;            ///< 状态
    std::size_t page_;                        ///< 页码
    std::size_t pageSize_;                    ///< 每页数据量

  public:
    UserQueryRequest(std::string username,
                     std::string nickname,
                     std::string sex,
                     std::string deptId,
                     std::string phoneNumber,
                     std::string email,
                     std::string status,
                     std::string page,
                     std::string pageSize);

    OPT_GETTER(username, Username)
    OPT_GETTER(nickname, Nickname)
    OPT_GETTER(sex, Sex)
    OPT_GETTER(deptId, DeptId)
    OPT_GETTER(phoneNumber, PhoneNumber)
    OPT_GETTER(email, Email)
    OPT_GETTER(status, Status)
    GETTER(page, Page)
    GETTER(pageSize, PageSize)
};
