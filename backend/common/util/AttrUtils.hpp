#pragma once

#include <drogon/Attribute.h>

inline void addWarning(const drogon::AttributesPtr &attr,
                       const std::string &warning)
{
    if (!attr->find("warning"))
    {
        attr->insert("warning", std::vector<std::string>{warning});
    }
    else
    {
        auto v = std::any_cast<std::vector<std::string>>((*attr)["warning"]);
        v.push_back(warning);
        attr->insert("warning", v);
    }
}
