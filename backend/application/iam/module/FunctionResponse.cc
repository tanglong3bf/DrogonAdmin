#include "FunctionResponse.h"

#include <json/value.h>

using namespace std;

Json::Value toJsonArray(const vector<FunctionResponse> &data)
{
    Json::Value array(Json::arrayValue);
    for (const auto &item : data)
    {
        array.append(item.toJson());
    }
    return array;
}

FunctionResponse::FunctionResponse(const Function &function)
    : functionId_{*function.functionId()},
      name_{function.name()},
      code_{function.code()},
      description_{function.description()},
      sortNum_{function.sortNum()},
      moduleId_{function.moduleId()}
{
}

Json::Value FunctionResponse::toJson() const
{
    Json::Value json;
    json["function_id"] = functionId_;
    json["name"] = name_;
    json["code"] = code_;
    if (description_)
    {
        json["description"] = *description_;
    }
    json["sort_num"] = sortNum_;
    json["module_id"] = moduleId_;
    return json;
}
