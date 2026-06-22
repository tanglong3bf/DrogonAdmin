#include "UserCreateRequest.h"

#include "common/util/ParamGetter.hpp"
#include "common/exception/BusinessException.h"
#include <drogon/HttpRequest.h>
#include <regex>

using namespace std;
using namespace drogon_admin::util;

namespace drogon
{
template <>
UserCreateRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    UserCreateRequest request(*jsonPtr);
    return request;
}
};  // namespace drogon

UserCreateRequest::UserCreateRequest(const Json::Value &json)
{
    username_ = getParam<string, true>(
        json,
        "username",
        {6, 20},
        [](const std::string &username) noexcept -> std::string {
            for (const char c : username)
            {
                if (!isalnum(static_cast<unsigned char>(c)) && c != '_')
                {
                    return "用户名只支持字母、数字和下划线";
                }
            }
            return "";
        });
    nickname_ = getParam<string, true>(json, "nickname", {1, 50});

    const auto sex = getParam<int8_t, true>(json, "sex", {0, 2});
    sex_ = static_cast<Sex>(sex);

    deptId_ = getParam<int32_t, true>(json, "dept_id", {0, -1});
    phoneNumber_ = getParam<string>(
        json,
        "phone_number",
        {11, 11},
        [](const std::string &phoneNumber) noexcept -> std::string {
            for (const char c : phoneNumber)
            {
                if (!isdigit(static_cast<unsigned char>(c)))
                {
                    return "手机号只支持数字";
                }
            }
            return "";
        });
    // 最短: a@b.c
    email_ = getParam<string>(
        json,
        "email",
        {5, -1},
        [](const std::string &email) noexcept -> std::string {
            static const std::regex email_regex(
                R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]+$)");

            if (std::regex_match(email, email_regex))
            {
                return "";
            }

            return "邮箱格式错误";
        });

    const auto status = getParam<int8_t, true>(json, "status", {0, 1});
    status_ = static_cast<Status>(status);

    roleIds_ = getParam<vector<int32_t>>(json, "role_ids");
}
