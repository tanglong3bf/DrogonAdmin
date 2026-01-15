#include "DeptCreateRequest.h"

#include <drogon/HttpRequest.h>
#include "common/util/ParamGetter.hpp"
#include "common/exception/BusinessException.h"

using namespace drogon_admin::util;

namespace drogon
{
template <>
DeptCreateRequest fromRequest(const HttpRequest &req)
{
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr)
    {
        throw BusinessException("请求体格式错误，请使用application/json");
    }
    DeptCreateRequest request;
    request.setByJson(*jsonPtr);
    return request;
}
};  // namespace drogon

void DeptCreateRequest::setByJson(const Json::Value &json)
{
    name_ = getParam<std::string, true>(json, "name");
    parentId_ = getParam<std::uint32_t>(json, "parent_id");
}
