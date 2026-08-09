#include "ActionPriorityResponse.h"

Json::Value toJsonArray(const std::vector<ActionPriorityResponse> &data)
{
    Json::Value array(Json::arrayValue);
    for (const auto &item : data)
    {
        array.append(item.toJson());
    }
    return array;
}

ActionPriorityResponse::ActionPriorityResponse(
    const ActionPriority &actionPriority)
    : highId_(actionPriority.highId()), lowId_(actionPriority.lowId())
{
}

Json::Value ActionPriorityResponse::toJson() const
{
    Json::Value json;
    json["high_id"] = highId_;
    json["low_id"] = lowId_;
    return json;
}
