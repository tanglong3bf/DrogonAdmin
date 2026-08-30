#pragma once

#include "common/util/Utilities.hpp"
#include <json/value.h>
#include <string>

/**
 * @brief 修改密码请求
 */
struct ChangePasswordRequest
{
    ChangePasswordRequest() = default;

    ChangePasswordRequest(const Json::Value &json);

    GETTER_STR_VIEW(oldPassword)
    GETTER_STR_VIEW(newPassword)
    GETTER(version)

  private:
    std::string oldPassword_;  ///< 旧密码
    std::string newPassword_;  ///< 新密码
    std::int32_t version_;     ///< 乐观锁版本号
};
