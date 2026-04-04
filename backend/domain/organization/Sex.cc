#include "Sex.h"

template <>
std::string toString(const Sex &sex)
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
    return "Sex::[error]";
}
