#include "RoleResponse.h"

#include <drogon/HttpResponse.h>
#include <jsoncpp/json/value.h>

using namespace std;

Json::Value toJsonArray(const vector<RoleResponse> &data)
{
    Json::Value array;
    for (const auto &item : data)
    {
        array.append(item.toJson());
    }
    return array;
}

Json::Value toJsonArray(const vector<RoleDeptResponse> &depts)
{
    Json::Value array{Json::arrayValue};
    for (const auto &dept : depts)
    {
        array.append(dept.toJson());
    }
    return array;
}

RoleResponse::RoleResponse(const Role &role)
    : roleId_{role.roleId() != nullopt ? *role.roleId() : -1},
      name_{role.name()},
      code_{role.code()},
      description_{role.description()},
      userQuota_{role.userQuota()},
      quotaType_{role.quotaType()},
      relationType_{role.relationType()},
      version_{role.version()}
{
}

Json::Value RoleResponse::toJson() const
{
    Json::Value json;
    json["role_id"] = roleId_;
    json["name"] = name_;
    json["code"] = code_;
    if (description_)
    {
        json["description"] = *description_;
    }
    json["quota_type"] = static_cast<int16_t>(quotaType_);
    if (userQuota_)
    {
        json["user_quota"] = *userQuota_;
    }
    json["relation_type"] = static_cast<int16_t>(relationType_);
    json["version"] = version_;
    json["depts"] = toJsonArray(depts_);

    return json;
}

void RoleResponse::addRoleDept(const RoleDeptResponse &roleDept)
{
    depts_.push_back(roleDept);
}
