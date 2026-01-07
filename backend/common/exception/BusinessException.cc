#include "BusinessException.h"

using namespace std;
using namespace drogon;

BusinessException::BusinessException(const std::string &message,
                                     const HttpStatusCode code)
    : message_{message}, code_{code}
{
}

const char *BusinessException::what() const noexcept
{
    return message_.c_str();
}

HttpStatusCode BusinessException::getCode() const
{
    return code_;
}
