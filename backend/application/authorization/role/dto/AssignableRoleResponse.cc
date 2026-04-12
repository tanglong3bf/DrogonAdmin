#include "AssignableRoleResponse.h"

#include <drogon/HttpResponse.h>

using namespace std;

namespace drogon
{
template <>
HttpResponsePtr toResponse(vector<AssignableRoleResponse> &&response)
{
    Json::Value json;
    json["data"] = Json::Value(Json::arrayValue);
    for (const auto &item : response)
    {
        json["data"].append(item.toJson());
    }
    return HttpResponse::newHttpJsonResponse(json);
}
};  // namespace drogon

AssignableRoleResponse::AssignableRoleResponse(const std::int32_t roleId,
                                               const string &name)
    : roleId_(roleId), name_(name)
{
}

Json::Value AssignableRoleResponse::toJson() const
{
    Json::Value value;
    value["role_id"] = roleId_;
    value["name"] = name_;
    return value;
}
