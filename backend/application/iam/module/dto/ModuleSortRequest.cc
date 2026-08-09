#include "ModuleSortRequest.h"

#include "common/util/ParamGetter.hpp"
#include "common/exception/BusinessException.h"
#include <drogon/HttpRequest.h>
#include <unordered_set>

using namespace std;
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
    parentId_ = getParam<std::int32_t, false>(json, "parent_id", {1, -1});
    moduleIds_ = getParam<vector<std::int32_t>, true>(
        json,
        "module_ids",
        {1, -1},
        [](const std::vector<std::int32_t> &moduleIds) noexcept -> std::string {
            // 进一步检查是否包含重复值
            const unordered_set<std::int32_t> idsSet{moduleIds.begin(),
                                                     moduleIds.end()};
            if (idsSet.size() != moduleIds.size())
            {
                return "module_ids参数包含重复值";
            }
            return "";
        });
}
