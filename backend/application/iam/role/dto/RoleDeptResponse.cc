#include "RoleDeptResponse.h"

using namespace std;

Json::Value RoleDeptResponse::toJson() const
{
    Json::Value json;
    json["role_id"] = roleId_;
    json["dept_id"] = deptId_;
    return json;
}

RoleDeptResponse::RoleDeptResponse(const RoleDept &roleDept)
    : roleId_{roleDept.roleId() != nullopt ? *roleDept.roleId() : -1},
      deptId_{roleDept.deptId()}
{
}
