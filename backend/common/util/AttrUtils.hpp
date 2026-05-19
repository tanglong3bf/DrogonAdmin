#pragma once

#include <drogon/Attribute.h>

inline void addWarn(const drogon::AttributesPtr &attr, const std::string &warn)
{
    if (!attr->find("warn"))
    {
        attr->insert("warn", std::vector<std::string>{warn});
    }
    else
    {
        auto v = std::any_cast<std::vector<std::string>>((*attr)["warn"]);
        v.push_back(warn);
        attr->insert("warn", v);
    }
}
