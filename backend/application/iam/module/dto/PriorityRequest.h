#pragma once

#include "common/util/Utilities.hpp"
#include <json/value.h>

class PriorityRequest
{
  public:
    PriorityRequest() = default;
    PriorityRequest(const Json::Value &json);

    GETTER(highId)
    GETTER(lowId)

  private:
    std::int64_t highId_;
    std::int64_t lowId_;
};
