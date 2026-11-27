#include "LoginResponse.h"

using namespace drogon;

namespace drogon
{
template <>
HttpResponsePtr toResponse(const LoginResponse &response)
{
    Json::Value json;
    json["data"] = response.toJson();
    return HttpResponse::newHttpJsonResponse(json);
}
};  // namespace drogon

Json::Value LoginResponse::toJson() const
{
    Json::Value json;
    json["token"] = token_;
    json["menu_list"] = toJsonArray(menuList_);
    json["user_info"] = user_.toJson();
    return json;
}
