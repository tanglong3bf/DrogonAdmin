#include "QuotaType.h"

#include <trantor/utils/Logger.h>

using namespace std;

template <>
string toString(const QuotaType &quotaType)
{
    switch (quotaType)
    {
        case QuotaType::Unlimited:
            return "Unlimited";
        case QuotaType::TotalLimit:
            return "TotalLimit";
        case QuotaType::PerDeptLimit:
            return "PerDeptLimit";
    }
    LOG_WARN << "错误的 QuotaType :" << static_cast<int32_t>(quotaType);
    return "QuotaType::[error]";
}
