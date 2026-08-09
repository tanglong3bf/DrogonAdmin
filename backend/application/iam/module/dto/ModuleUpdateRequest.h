#pragma once

#include "common/util/ParamGetter.hpp"
#include "common/util/Utilities.hpp"
#include <json/value.h>
#include <optional>

struct ModuleUpdateRequest
{
    ModuleUpdateRequest() = default;
    ModuleUpdateRequest(const Json::Value &json);

    GETTER(name)
    GETTER(description)

  private:
    std::optional<std::string> name_;
    drogon_admin::util::NullableValue<std::string> description_;
};
