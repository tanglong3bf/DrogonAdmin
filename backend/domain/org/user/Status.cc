#include "Status.h"

#include <trantor/utils/Logger.h>

using namespace std;

string toString(const Status &status)
{
    switch (status)
    {
        case Status::Enabled:
            return "Enabled";
        case Status::Disabled:
            return "Disabled";
    }
    LOG_ERROR << "错误的 Status :" << static_cast<int32_t>(status);
    return "Status::[error]";
}
