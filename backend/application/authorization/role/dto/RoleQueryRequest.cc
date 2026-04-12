#include "RoleQueryRequest.h"

#include <drogon/utils/Utilities.h>

using namespace drogon::utils;

RoleQueryRequest::RoleQueryRequest(std::string name,
                                   std::string deptId,
                                   std::string page,
                                   std::string pageSize)
{
    if (name.size() > 0)
    {
        name_ = name;
    }
    if (deptId.size() > 0)
    {
        deptId_ = fromString<int32_t>(deptId);
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
