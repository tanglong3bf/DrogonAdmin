#pragma once

#include <jsoncpp/json/value.h>
#include "common/util/Utilities.hpp"
#include "domain/authorization/DeptScopeType.h"
#include "domain/authorization/QuotaType.h"

class RoleUpdateRequest
{
    std::optional<std::string> name_;
    std::optional<std::string> code_;
    std::optional<std::string> description_;
    std::optional<QuotaType> quotaType_;
    std::optional<int32_t> userQuota_;
    std::optional<RelationType> relationType_;
    std::optional<std::vector<int32_t>> deptIds_;

  public:
    void setByJson(const Json::Value &json);

    OPT_GETTER(name, Name)
    OPT_GETTER(code, Code)
    OPT_GETTER(description, Description)
    OPT_GETTER(quotaType, QuotaType)
    OPT_GETTER(userQuota, UserQuota)
    OPT_GETTER(relationType, RelationType)
    OPT_GETTER(deptIds, DeptIds)
};
