#include "ActionUpdateRequest.h"

#include "common/exception/BusinessException.h"
#include <drogon/HttpRequest.h>

using namespace std;

namespace drogon
{
template <>
ActionUpdateRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    ActionUpdateRequest request(*jsonPtr);
    return request;
}
};  // namespace drogon

ActionUpdateRequest::ActionUpdateRequest(const Json::Value &json)
{
    if (json.isMember("actions") &&
        json["actions"].type() == Json::arrayValue &&
        json["actions"].size() > 0)
    {
        for (const auto &item : json["actions"])
        {
            actions_.emplace_back(item);
        }
    }
    if (json.isMember("priorities") &&
        json["priorities"].type() == Json::arrayValue &&
        json["priorities"].size() > 0)
    {
        for (const auto &item : json["priorities"])
        {
            priorities_.emplace_back(item);
        }
    }
}
