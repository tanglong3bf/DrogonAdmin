#include "RoleQueryRequest.h"

#include <drogon/utils/Utilities.h>

using namespace drogon::utils;

RoleQueryRequest::RoleQueryRequest(const std::string &name,
                                   const std::string &deptId,
                                   const std::string &page,
                                   const std::string &pageSize)
{
    if (name.size() > 0)
    {
        name_ = name;
    }

    if (deptId.size() > 0)
    {
        deptId_ = fromString<int32_t>(deptId);
    }

    page_ = parseInt(page, 1);
    pageSize_ = parseInt(pageSize, 10);
}
