#include "RoleDeptResponse.h"

using namespace std;

Json::Value RoleDeptResponse::toJson() const
{
    Json::Value json;
    json["id"] = id_;
    json["role_id"] = roleId_;
    json["dept_id"] = deptId_;
    return json;
}

RoleDeptResponse::RoleDeptResponse(const RoleDept &roleDept)
    : id_{roleDept.getId() != nullopt ? *roleDept.getId() : -1},
      roleId_{roleDept.getRoleId() != nullopt ? *roleDept.getRoleId() : -1},
      deptId_{roleDept.getDeptId()}
{
}
