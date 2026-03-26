#include "GetRoleListRequest.h"

#include <drogon/HttpRequest.h>
#include "common/exception/BusinessException.h"

using namespace std;
using namespace drogon::utils;

namespace drogon
{
template <>
GetRoleListRequest fromRequest(const HttpRequest &req)
{
    GetRoleListRequest request;
    request.setByReq(req);
    return request;
}
};  // namespace drogon

void GetRoleListRequest::setByReq(const drogon::HttpRequest &req)
{
    if (req.getParameter("name") != "")
    {
        name_ = req.getParameter("name");
    }
    if (req.getParameter("dept_id") != "")
    {
        try
        {
            deptId_ = fromString<int32_t>(req.getParameter("deptId"));
        }
        catch (const exception &e)
        {
            LOG_ERROR << e.what();
            throw BusinessException("deptId参数格式错误");
        }
    }
    if (req.getParameter("page") != "")
    {
        try
        {
            page_ = fromString<int32_t>(req.getParameter("page"));
            if (page_ <= 0)
            {
                page_ = 1;
            }
        }
        catch (const exception & /* ignore */)
        {
        }
    }
    if (req.getParameter("page_size") != "")
    {
        try
        {
            pageSize_ = fromString<int32_t>(req.getParameter("page_size"));
            if (pageSize_ <= 0)
            {
                pageSize_ = 10;
            }
        }
        catch (const exception & /* ignore */)
        {
            pageSize_ = 10;
        }
        return;
    }
    if (pageSize_ < 20)
    {
        pageSize_ = 10;
    }
    else if (pageSize_ < 30)
    {
        pageSize_ = 20;
    }
    else if (pageSize_ < 50)
    {
        pageSize_ = 30;
    }
    else if (pageSize_ < 100)
    {
        pageSize_ = 50;
    }
    else
    {
        pageSize_ = 100;
    }
}
