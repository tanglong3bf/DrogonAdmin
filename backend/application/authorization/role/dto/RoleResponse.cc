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
    : roleId_{role.getRoleId() != nullopt ? *role.getRoleId() : -1},
      name_{role.getName()},
      code_{role.getCode()},
      description_{role.getDescription()},
      userQuota_{role.getUserQuota()},
      quotaType_{role.getQuotaType()},
      relationType_{role.getRelationType()}
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
    json["depts"] = toJsonArray(depts);

    return json;
}

void RoleResponse::addRoleDept(const RoleDeptResponse &roleDept)
{
    depts.push_back(roleDept);
}
