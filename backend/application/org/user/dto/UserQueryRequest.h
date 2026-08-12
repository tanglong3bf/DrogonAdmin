#pragma once

#include "domain/org/user/Sex.h"
#include "domain/org/user/Status.h"
#include "common/util/Utilities.hpp"
#include <drogon/Attribute.h>
#include <optional>
#include <string>
#include <cstdint>

/**
 * 用户查询请求
 */
struct UserQueryRequest
{
    UserQueryRequest(const std::string &username,
                     const std::string &nickname,
                     const std::string &sex,
                     const std::string &deptId,
                     const std::string &phoneNumber,
                     const std::string &email,
                     const std::string &status,
                     const std::string &page,
                     const std::string &pageSize,
                     const drogon::AttributesPtr &attr);

    GETTER(username)
    GETTER(nickname)
    GETTER(sex)
    GETTER(deptId)
    GETTER(phoneNumber)
    GETTER(email)
    GETTER(status)
    GETTER(page)
    GETTER(pageSize)

  private:
    std::optional<std::string> username_;     ///< 用户名
    std::optional<std::string> nickname_;     ///< nicheng
    std::optional<Sex> sex_;                  ///< 性别
    std::optional<std::int32_t> deptId_;      ///< 部门
    std::optional<std::string> phoneNumber_;  ///< 手机号
    std::optional<std::string> email_;        ///< 邮箱
    std::optional<Status> status_;            ///< 状态
    std::size_t page_;                        ///< 页码
    std::size_t pageSize_;                    ///< 每页数据量
};
