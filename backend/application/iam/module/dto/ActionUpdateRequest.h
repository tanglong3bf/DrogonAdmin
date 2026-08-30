#pragma once

#include "ActionRequest.h"
#include "PriorityRequest.h"
#include <json/value.h>

class ActionUpdateRequest
{
  public:
    ActionUpdateRequest() = default;
    ActionUpdateRequest(const Json::Value &json);

    GETTER(actions)
    GETTER(priorities)
    GETTER(version)

  private:
    std::vector<ActionRequest> actions_;
    std::vector<PriorityRequest> priorities_;
    std::int32_t version_;
};
