#include "UserInfoUpdateRequest.h"

#include "common/util/ParamGetter.hpp"
#include <drogon/HttpRequest.h>
#include <regex>

using namespace std;
using namespace drogon_admin::util;

namespace drogon
{
template <>
UserInfoUpdateRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    UserInfoUpdateRequest request(*jsonPtr);
    return request;
}
};  // namespace drogon

UserInfoUpdateRequest::UserInfoUpdateRequest(const Json::Value &json)
{
    nickname_ = getParam<std::string>(json, "nickname", {1, 50});
    auto sexInt = getParam<int32_t>(json, "sex", {0, 2});
    if (sexInt)
    {
        sex_ = static_cast<Sex>(*sexInt);
    }

    auto phoneNumberStr = getParam<string, false, true>(
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
    phoneNumber_ = phoneNumberStr.to<PhoneNumber>();

    auto emailStr = getParam<string, false, true>(
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
    email_ = emailStr.to<Email>();
}
