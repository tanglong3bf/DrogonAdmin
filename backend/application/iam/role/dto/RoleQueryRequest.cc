#include "RoleQueryRequest.h"

#include "common/exception/BusinessException.h"
#include "common/util/AttrUtils.hpp"
#include "common/util/ParamUtils.hpp"
#include <drogon/utils/Utilities.h>
#include <trantor/utils/Logger.h>

using namespace std;
using namespace drogon;
using namespace drogon::utils;

RoleQueryRequest::RoleQueryRequest(const std::string &name,
                                   const std::string &deptId,
                                   const std::string &page,
                                   const std::string &pageSize,
                                   const AttributesPtr &attr)
    : name_{nullopt}, deptId_{nullopt}, page_{1}, pageSize_{10}
{
    if (name.size() > 0)
    {
        auto trimed = trim(name);
        name_ = escapeSqlLike(trimed);
    }

    if (deptId.size() > 0)
    {
        try
        {
            auto deptIdInt = fromString<int32_t>(deptId);
            if (deptIdInt > 0)
            {
                deptId_ = deptIdInt;
            }
            else
            {
                throw BusinessException("部门id范围错误");
            }
        }
        catch (std::invalid_argument e)
        {
            throw BusinessException("部门id格式错误");
        }
    }
    if (page.size() > 0)
    {
        try
        {
            auto pageInt = fromString<int32_t>(page);
            if (pageInt <= 0)
            {
                addWarning(attr, "查询页码超出范围，已自动调整到第一页");
                pageInt = 1;
            }
            page_ = pageInt;
        }
        catch (std::invalid_argument e)
        {
            addWarning(attr, "页码格式错误，已自动使用第一页");
            page_ = 1;
        }
    }
    if (pageSize.size() > 0)
    {
        try
        {
            auto pageSizeInt = fromString<int32_t>(pageSize);
            if (pageSizeInt < 20)
            {
                if (pageSizeInt != 10)
                {
                    addWarning(attr, "不支持的分页大小，已自动调整为10");
                }
                pageSizeInt = 10;
            }
            else if (pageSizeInt < 30)
            {
                if (pageSizeInt != 20)
                {
                    addWarning(attr, "不支持的分页大小，已自动调整为20");
                }
                pageSizeInt = 20;
            }
            else if (pageSizeInt < 50)
            {
                if (pageSizeInt != 30)
                {
                    addWarning(attr, "不支持的分页大小，已自动调整为30");
                }
                pageSizeInt = 30;
            }
            else if (pageSizeInt < 100)
            {
                if (pageSizeInt != 50)
                {
                    addWarning(attr, "不支持的分页大小，已自动调整为50");
                }
                pageSizeInt = 50;
            }
            else
            {
                if (pageSizeInt != 100)
                {
                    addWarning(attr, "不支持的分页大小，已自动调整为100");
                }
                pageSizeInt = 100;
            }
            pageSize_ = pageSizeInt;
        }
        catch (std::invalid_argument e)
        {
            addWarning(attr, "分页大小格式错误，已自动调整为10");
            pageSize_ = 10;
        }
    }
}
