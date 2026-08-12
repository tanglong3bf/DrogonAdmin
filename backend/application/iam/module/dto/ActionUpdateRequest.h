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

  private:
    std::vector<ActionRequest> actions_;
    std::vector<PriorityRequest> priorities_;
};
