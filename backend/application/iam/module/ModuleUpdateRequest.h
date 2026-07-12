#pragma once

#include "common/util/ParamGetter.hpp"
#include "common/util/Utilities.hpp"
#include <json/value.h>
#include <optional>

using namespace std;
using namespace drogon_admin::util;

struct ModuleUpdateRequest
{
    ModuleUpdateRequest() = default;
    ModuleUpdateRequest(const Json::Value &json);

    GETTER(name)
    GETTER(description)

  private:
    optional<string> name_;
    NullableValue<string> description_;
};
