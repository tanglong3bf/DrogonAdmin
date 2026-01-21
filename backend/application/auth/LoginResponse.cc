#include "LoginResponse.h"

#include <drogon/HttpAppFramework.h>

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
    // TODO: 后续改为AuthService查询并传入
    const auto config = app().getCustomConfig();
    json["menu_list"] = config["menu_list"];
    json["user_info"] = config["user_info"];
    return json;
}
