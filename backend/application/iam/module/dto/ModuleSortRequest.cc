#include "ModuleSortRequest.h"

#include "common/util/ParamGetter.hpp"
#include "common/util/rangesUtils.hpp"
#include "common/exception/BusinessException.h"
#include <drogon/HttpRequest.h>
#include <ranges>

using namespace std;
using namespace drogon_admin;
using namespace drogon_admin::util;

namespace drogon
{
template <>
ModuleSortRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    ModuleSortRequest request(*jsonPtr);
    return request;
}
};  // namespace drogon

ModuleSortRequest::ModuleSortRequest(const Json::Value &json)
{
    parentId_ = getParam<int32_t, false>(json, "parent_id", {1, -1});
    if (json.isMember("modules") && json["modules"].isArray())
    {
        size_t i = 0;
        for (const auto &jsonItem : json["modules"])
        {
            ModuleSortItem item;
            item.moduleId = getParam<int32_t, true>(jsonItem, "module_id");
            item.version = getParam<int32_t, true>(jsonItem, "version");
            modules_.emplace_back(item);
        }
    }
    if (modules_.size() == 0)
    {
        throw BusinessException("缺少必备参数：modules");
    }
}

vector<int32_t> ModuleSortRequest::moduleIds() const
{
    return modules_ |
           views::transform([](const auto &item) { return item.moduleId; }) |
           ranges_utils::to<vector>();
}
