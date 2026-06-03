#include "UserQueryRequest.h"

#include <drogon/utils/Utilities.h>

using namespace std;
using namespace drogon;
using namespace drogon::utils;

UserQueryRequest::UserQueryRequest(const string &username,
                                   const string &nickname,
                                   const string &sex,
                                   const string &deptId,
                                   const string &phoneNumber,
                                   const string &email,
                                   const string &status,
                                   const string &page,
                                   const string &pageSize)
{
    if (!username.empty())
    {
        username_ = username;
    }
    if (!nickname.empty())
    {
        nickname_ = nickname;
    }
    if (sex.size() == 1 && fromString<int32_t>(sex) >= 0 &&
        fromString<int32_t>(sex) <= 2)
    {
        sex_ = static_cast<Sex>(fromString<int32_t>(sex));
    }
    if (!deptId.empty())
    {
        deptId_ = fromString<int32_t>(deptId);
    }
    if (!phoneNumber.empty())
    {
        phoneNumber_ = phoneNumber;
    }
    if (!email.empty())
    {
        email_ = email;
    }
    if (status.size() == 1 && fromString<int32_t>(status) >= 0 &&
        fromString<int32_t>(status) <= 2)
    {
        status_ = static_cast<Status>(fromString<int32_t>(status));
    }
    page_ = (!page.empty() && fromString<int32_t>(page) > 0)
                ? fromString<int32_t>(page)
                : 1;
    pageSize_ = (!pageSize.empty() && fromString<int32_t>(pageSize) > 0)
                    ? fromString<int32_t>(pageSize)
                    : 10;
}
