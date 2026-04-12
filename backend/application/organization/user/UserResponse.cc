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
    : userId_{*user.getUserId()},
      username_{user.getUsername()},
      nickname_{user.getNickname()},
      avatar_{user.getAvatar()},
      sex_{user.getSex()},
      deptId_{user.getDeptId()},
      phoneNumber_{user.getPhoneNumber()},
      email_{user.getEmail()},
      status_{user.getStatus()}
{
    if (user.getUserRoles().size() > 0)
    {
        userRoles_.reserve(user.getUserRoles().size());

        for (const auto &userRole : user.getUserRoles())
        {
            userRoles_.push_back(UserRoleResponse{userRole});
        }
    }
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
        json["phone_number"] = phoneNumber_->value();
    }
    if (email_)
    {
        json["email"] = email_->value();
    }
    json["status"] = static_cast<int8_t>(status_);
    if (userRoles_.size() > 0)
    {
        json["user_roles"] = Json::Value(Json::arrayValue);
        for (const auto &userRole : userRoles_)
        {
            json["user_roles"].append(userRole.toJson());
        }
    }
    return json;
}
