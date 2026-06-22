#include "UserQueryRequest.h"

#include "common/exception/BusinessException.h"
#include "common/util/ParamUtils.hpp"
#include "common/util/AttrUtils.hpp"
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
                                   const string &pageSize,
                                   const AttributesPtr &attr)
    : username_{nullopt},
      nickname_{nullopt},
      sex_{nullopt},
      deptId_{nullopt},
      phoneNumber_{nullopt},
      email_{nullopt},
      status_{nullopt},
      page_{1},
      pageSize_{10}
{
    if (username.size() > 0)
    {
        auto trimed = trim(username);
        username_ = escapeSqlLike(trimed);
    }
    if (nickname.size() > 0)
    {
        auto trimed = trim(nickname);
        nickname_ = escapeSqlLike(trimed);
    }
    if (sex.size() == 1)
    {
        try
        {
            const auto sexInt = fromString<int32_t>(sex);
            if (sexInt >= 0 && sexInt <= 2) [[likely]]
            {
                sex_ = static_cast<Sex>(sexInt);
            }
            else
            {
                throw BusinessException("性别范围错误");
            }
        }
        catch (std::invalid_argument e)
        {
            throw BusinessException("性别格式错误");
        }
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
    if (phoneNumber.size() > 0)
    {
        auto trimed = trim(phoneNumber);
        phoneNumber_ = escapeSqlLike(trimed);
    }
    if (email.size() > 0)
    {
        auto trimed = trim(email);
        email_ = escapeSqlLike(trimed);
    }
    if (status.size() > 0)
    {
        try
        {
            const auto statusInt = fromString<int32_t>(status);
            if (statusInt >= 0 && statusInt <= 1) [[likely]]
            {
                status_ = static_cast<Status>(statusInt);
            }
            else
            {
                throw BusinessException("状态范围错误");
            }
        }
        catch (std::invalid_argument e)
        {
            throw BusinessException("状态格式错误");
        }
    }
    if (page.size() > 0)
    {
        try
        {
            auto pageInt = fromString<int32_t>(page);
            if (pageInt <= 0)
            {
                addWarn(attr, "查询页码超出范围，已自动调整到第一页");
                pageInt = 1;
            }
            page_ = pageInt;
        }
        catch (std::invalid_argument e)
        {
            addWarn(attr, "页码格式错误，已自动使用第一页");
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
                    addWarn(attr, "不支持的分页大小，已自动调整为10");
                }
                pageSizeInt = 10;
            }
            else if (pageSizeInt < 30)
            {
                if (pageSizeInt != 20)
                {
                    addWarn(attr, "不支持的分页大小，已自动调整为20");
                }
                pageSizeInt = 20;
            }
            else if (pageSizeInt < 50)
            {
                if (pageSizeInt != 30)
                {
                    addWarn(attr, "不支持的分页大小，已自动调整为30");
                }
                pageSizeInt = 30;
            }
            else if (pageSizeInt < 100)
            {
                if (pageSizeInt != 50)
                {
                    addWarn(attr, "不支持的分页大小，已自动调整为50");
                }
                pageSizeInt = 50;
            }
            else
            {
                if (pageSizeInt != 100)
                {
                    addWarn(attr, "不支持的分页大小，已自动调整为100");
                }
                pageSizeInt = 100;
            }
            pageSize_ = pageSizeInt;
        }
        catch (std::invalid_argument e)
        {
            addWarn(attr, "分页大小格式错误，已自动调整为10");
            pageSize_ = 10;
        }
    }
}
