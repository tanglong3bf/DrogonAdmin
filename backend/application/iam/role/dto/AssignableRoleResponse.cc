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
                                               const string &name,
                                               const int32_t version)
    : roleId_{roleId}, name_{name}, version_{version}
{
}

Json::Value AssignableRoleResponse::toJson() const
{
    Json::Value value;
    value["role_id"] = roleId_;
    value["name"] = name_;
    value["version"] = version_;
    return value;
}
