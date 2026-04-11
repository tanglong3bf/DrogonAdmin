#include "DeptScopeType.h"

#include <trantor/utils/Logger.h>

using namespace std;

template <>
string toString(const RelationType &relationType)
{
    switch (relationType)
    {
        case RelationType::All:
            return "All";
        case RelationType::Whitelist:
            return "Whitelist";
        case RelationType::Blacklist:
            return "Blacklist";
    }
    LOG_WARN << "错误的 RelationType :" << static_cast<int32_t>(relationType);
    return "RelationType::[error]";
}
