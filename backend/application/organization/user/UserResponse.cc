#include "UserResponse.h"
#include <drogon/HttpResponse.h>

namespace drogon
{
template <>
HttpResponsePtr toResponse(const UserResponse &response)
{
    Json::Value json;
    json["data"] = response.toJson();
    return HttpResponse::newHttpJsonResponse(json);
}
};  // namespace drogon

UserResponse::UserResponse(const User &user)
    : userId_{user.getDeptId()},
      username_{user.getUsername()},
      nickname_{user.getNickname()},
      avatar_{user.getAvatar()},
      sex_{user.getSex()},
      deptId_{user.getDeptId()},
      phoneNumber_{user.getPhoneNumber()},
      email_{user.getEmail()},
      status_{user.getStatus()}
{
}

Json::Value UserResponse::toJson() const
{
    Json::Value json;
    json["user_id"] = userId_;
    json["username"] = username_;
    json["nickname"] = nickname_;
    json["avatar"] = avatar_;
    json["sex"] = static_cast<int8_t>(sex_);
    json["dept_id"] = deptId_;
    if (phoneNumber_)
    {
        json["phone_number"] = *phoneNumber_;
    }
    if (email_)
    {
        json["email"] = *email_;
    }
    json["status"] = static_cast<int8_t>(status_);
    return json;
}
