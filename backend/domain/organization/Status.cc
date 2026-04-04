#include "Status.h"

template <>
std::string toString(const Status &status)
{
    switch (status)
    {
        case Status::Enabled:
            return "Enabled";
        case Status::Disabled:
            return "Disabled";
    }
    return "Status::[error]";
}
