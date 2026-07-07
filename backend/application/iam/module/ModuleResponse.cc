#include "ModuleResponse.h"

#include <drogon/HttpResponse.h>

using namespace std;

Json::Value toJsonArray(const vector<FunctionResponse> &data);

Json::Value toJsonArray(const vector<ModuleResponse> &data)
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
HttpResponsePtr toResponse(const vector<ModuleResponse> &response)
{
    Json::Value json;
    json["data"] = toJsonArray(response);
    return HttpResponse::newHttpJsonResponse(json);
}
};  // namespace drogon

ModuleResponse::ModuleResponse(const Module &module)
    : moduleId_(*module.moduleId()),
      name_(module.name()),
      description_(module.description()),
      sortNum_(module.sortNum()),
      parentId_(module.parentId())
{
}

Json::Value ModuleResponse::toJson() const
{
    Json::Value json;
    json["module_id"] = moduleId_;
    json["name"] = name_;
    if (description_)
    {
        json["description"] = *description_;
    }
    json["sort_num"] = sortNum_;
    if (parentId_)
    {
        json["parent_id"] = *parentId_;
    }
    if (children_.size() > 0)
    {
        json["children"] = toJsonArray(children_);
    }
    if (functions_.size() > 0)
    {
        json["functions"] = toJsonArray(functions_);
    }
    return json;
}

void ModuleResponse::appendFunction(FunctionResponse &func)
{
    functions_.emplace_back(func);
}

void ModuleResponse::addChild(ModuleResponse &child)
{
    assert(child.parentId_ == moduleId_);
    if (!isChildIdExist(child.moduleId_))
    {
        children_.emplace_back(child);
        child.parent_ = this;
    }
}

bool ModuleResponse::isChildIdExist(const std::int32_t id) noexcept
{
    for (const auto &child : children_)
    {
        if (child.moduleId_ == id)
        {
            return true;
        }
    }
    return false;
}
