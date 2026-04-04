#include "UserQueryRequest.h"
#include <drogon/utils/Utilities.h>

using namespace drogon;
using namespace drogon::utils;

UserQueryRequest::UserQueryRequest(std::string username,
                                   std::string nickname,
                                   std::string sex,
                                   std::string deptId,
                                   std::string phoneNumber,
                                   std::string email,
                                   std::string status,
                                   std::string page,
                                   std::string pageSize)
{
    if (username.size() > 0)
    {
        username_ = username;
    }
    if (nickname.size() > 0)
    {
        nickname_ = nickname;
    }
    if (sex.size() == 1)
    {
        const auto sexInt = fromString<int32_t>(sex);
        if (sexInt >= 0 && sexInt <= 2)
        {
            sex_ = static_cast<Sex>(sexInt);
        }
    }
    if (deptId.size() > 0)
    {
        deptId_ = fromString<int32_t>(deptId);
    }
    if (phoneNumber.size() > 0)
    {
        phoneNumber_ = phoneNumber;
    }
    if (email.size() > 0)
    {
        email_ = email;
    }
    if (status.size() > 0)
    {
        const auto statusInt = fromString<int32_t>(status);
        if (statusInt >= 0 && statusInt <= 2)
        {
            status_ = static_cast<Status>(statusInt);
        }
    }
    if (page.size() > 0)
    {
        page_ = fromString<int32_t>(page);
    }
    if (page_ <= 0)
    {
        page_ = 1;
    }
    if (pageSize.size() > 0)
    {
        pageSize_ = fromString<int32_t>(pageSize);
    }
    if (pageSize_ <= 0)
    {
        pageSize_ = 10;
    }
}
