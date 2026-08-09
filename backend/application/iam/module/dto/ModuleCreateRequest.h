#pragma once

#include <json/value.h>
#include <optional>
#include "common/util/Utilities.hpp"

struct ModuleCreateRequest
{
    ModuleCreateRequest() = default;
    ModuleCreateRequest(const Json::Value &json);

    GETTER_STR_VIEW(name)
    GETTER(description)
    GETTER(parentId)

  private:
    std::string name_;
    std::optional<std::string> description_;
    std::optional<std::int32_t> parentId_;
};
