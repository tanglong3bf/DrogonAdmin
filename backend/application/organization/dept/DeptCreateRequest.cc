#include "DeptCreateRequest.h"

#include "common/exception/BusinessException.h"
#include "common/util/ParamGetter.hpp"
#include <drogon/HttpRequest.h>

using namespace std;
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
    DeptCreateRequest request(*jsonPtr);
    return request;
}
};  // namespace drogon

DeptCreateRequest::DeptCreateRequest(const Json::Value &json)
{
    name_ = getParam<string, true>(json, "name");
    parentId_ = getParam<int32_t>(json, "parent_id");
}
