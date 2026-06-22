#include "DeptResponse.h"

#include <drogon/HttpResponse.h>

using namespace std;

Json::Value toJsonArray(const vector<DeptResponse> &data)
{
    Json::Value array(Json::arrayValue);
    for (const auto &item : data)
    {
        array.append(item.toJson());
    }
    return array;
}

namespace drogon
{
template <>
HttpResponsePtr toResponse(const vector<DeptResponse> &response)
{
    Json::Value json;
    json["data"] = toJsonArray(response);
    return HttpResponse::newHttpJsonResponse(json);
}
};  // namespace drogon

Json::Value DeptResponse::toJson() const
{
    Json::Value result;
    result["dept_id"] = deptId_;
    result["name"] = name_;
    result["sort_num"] = sortNum_;
    if (parentId_)
    {
        result["parent_id"] = *parentId_;
    }
    if (children_.size() > 0)
    {
        result["children"] = toJsonArray(children_);
    }
    return result;
}

DeptResponse::DeptResponse(const Dept &dept)
    : deptId_{*dept.deptId},
      name_{dept.name},
      sortNum_{dept.sortNum},
      parentId_{dept.parentId}
{
}

void DeptResponse::addChild(DeptResponse &child)
{
    assert(child.parentId_ == deptId_);
    if (!isChildIdExist(child.deptId_))
    {
        children_.emplace_back(child);
        child.parent_ = this;
    }
}

bool DeptResponse::isChildIdExist(const std::int32_t id) noexcept
{
    for (const auto &child : children_)
    {
        if (child.deptId_ == id)
        {
            return true;
        }
    }
    return false;
}
