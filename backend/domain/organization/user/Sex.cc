#include "Sex.h"

#include <trantor/utils/Logger.h>

using namespace std;

string toString(const Sex &sex)
{
    switch (sex)
    {
        case Sex::Unknown:
            return "Unknown";
        case Sex::Male:
            return "Male";
        case Sex::Female:
            return "Female";
    }
    LOG_ERROR << "错误的 Sex :" << static_cast<int32_t>(sex);
    return "Sex::[error]";
}
