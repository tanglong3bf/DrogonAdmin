#include "ActionUpdateRequest.h"

#include "common/exception/BusinessException.h"
#include "common/util/ParamGetter.hpp"
#include "common/util/rangesUtils.hpp"
#include <drogon/HttpRequest.h>
#include <utility>
#include <ranges>

using namespace std;
using namespace drogon_admin;
using namespace drogon_admin::util;

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
        for (const auto &itemInReq : json["actions"])
        {
            // 参数合法性检查
            for (const auto &itemProcessed : actions_)
            {
                if (itemProcessed.actionId() ==
                    itemInReq["action_id"].asInt64())
                {
                    throw BusinessException("actions中存在重复项");
                }
                else if (itemProcessed.code() == itemInReq["code"].asString())
                {
                    throw BusinessException("请求体中存在重复的code");
                }
                else if (itemProcessed.name() == itemInReq["name"].asString())
                {
                    throw BusinessException("请求体中存在重复的name");
                }
            }
            actions_.emplace_back(itemInReq);
        }
    }
    if (json.isMember("priorities") &&
        json["priorities"].type() == Json::arrayValue &&
        json["priorities"].size() > 0)
    {
        for (const auto &itemInReq : json["priorities"])
        {
            // 参数合法性检查
            for (const auto &itemProcessed : priorities_)
            {
                if (itemProcessed.highId() == itemInReq["high_id"].asInt64() &&
                    itemProcessed.lowId() == itemInReq["low_id"].asInt64())
                {
                    throw BusinessException("priorities中存在重复项");
                }
            }
            priorities_.emplace_back(itemInReq);
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

    version_ = getParam<int32_t, true>(json, "version", {0, -1});
}
