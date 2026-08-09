#pragma once

#include "common/util/Utilities.hpp"
#include <json/value.h>

class ActionRequest
{
  public:
    ActionRequest() = default;
    ActionRequest(const Json::Value &json);

    GETTER(actionId)
    GETTER(name)
    GETTER(code)
    GETTER(moduleId)
    GETTER(hasDataPermission)
    GETTER(sortNum)

  private:
    std::int64_t actionId_;
    std::string name_;
    std::string code_;
    std::int32_t moduleId_;
    bool hasDataPermission_;
    std::int32_t sortNum_;
};
