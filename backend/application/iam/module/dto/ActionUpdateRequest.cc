#include "ActionUpdateRequest.h"

#include "common/util/rangesUtils.hpp"
#include "common/exception/BusinessException.h"
#include <drogon/HttpRequest.h>
#include <utility>
#include <ranges>

using namespace std;
using namespace drogon_admin;

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
    // 参数合法性检查
    if (actions_.size() == 0 && priorities_.size() > 0)
    {
        throw BusinessException("actions为空时，priorities也应为空");
    }
    map<int64_t, int32_t> sortsMap;
    if (actions_.size() > 0)
    {
        sortsMap = actions_ | views::transform([](const ActionRequest &a) {
                       return make_pair(a.actionId(), a.sortNum());
                   }) |
                   ranges_utils::to<map<int64_t, int32_t>>();
    }
    if (priorities_.size() > 0)
    {
        for (const PriorityRequest &p : priorities_)
        {
            const auto highIter = sortsMap.find(p.highId());
            const auto lowIter = sortsMap.find(p.lowId());
            if (highIter == sortsMap.end() || lowIter == sortsMap.end())
            {
                throw BusinessException("priorities中的action_id不存在");
            }
            if (highIter->second > lowIter->second)
            {
                throw BusinessException("权限优先级应该满足排序的条件");
            }
        }
    }
}
