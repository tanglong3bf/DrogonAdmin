#include "ActionResponse.h"

#include <json/value.h>

using namespace std;

Json::Value toJsonArray(const vector<ActionResponse> &data)
{
    Json::Value array(Json::arrayValue);
    for (const auto &item : data)
    {
        array.append(item.toJson());
    }
    return array;
}

ActionResponse::ActionResponse(const Action &action)
    : actionId_{*action.actionId()},
      name_{action.name()},
      code_{action.code()},
      description_{action.description()},
      sortNum_{action.sortNum()},
      moduleId_{action.moduleId()}
{
}

Json::Value ActionResponse::toJson() const
{
    Json::Value json;
    json["action_id"] = actionId_;
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
