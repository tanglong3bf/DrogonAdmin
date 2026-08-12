#include "PriorityRequest.h"

#include "common/util/ParamGetter.hpp"

using namespace drogon_admin::util;

PriorityRequest::PriorityRequest(const Json::Value &json)
{
    highId_ = getParam<std::int64_t, true>(json, "high_id");
    lowId_ = getParam<std::int64_t, true>(json, "low_id");
}
