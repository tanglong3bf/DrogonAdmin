#include "application/organization/user/UserCqrsRepo.h"

#include <drogon/HttpAppFramework.h>
#include <drogon/orm/Criteria.h>
#include "domain/models/SysUser.h"

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::drogon_admin_db;

Task<size_t> UserCqrsRepo::countByQueryReq(
    const UserQueryRequest &request) const
{
    Criteria criteria = buildCriteria(request);
    co_return co_await userMapper().count(criteria);
}

Task<vector<UserResponse>> UserCqrsRepo::getUserList(
    const UserQueryRequest &request,
    const int32_t maxPage) const
{
    Criteria criteria = buildCriteria(request);

    const int32_t page =
        maxPage < request.getPage() ? maxPage : request.getPage();
    const auto sysUserList = co_await userMapper()
                                 .paginate(page, request.getPageSize())
                                 .findBy(criteria);
    co_return buildUserList(sysUserList);
}

Criteria UserCqrsRepo::buildCriteria(const UserQueryRequest &request) const
{
    Criteria criteria{SysUser::Cols::_deleted_by, CompareOperator::IsNull};
    if (request.getUsername())
    {
        criteria = criteria && Criteria{SysUser::Cols::_username,
                                        CompareOperator::Like,
                                        "%" + *request.getUsername() + "%"};
    }
    if (request.getNickname())
    {
        criteria = criteria && Criteria{SysUser::Cols::_nickname,
                                        CompareOperator::Like,
                                        "%" + *request.getNickname() + "%"};
    }
    if (request.getSex())
    {
        criteria =
            criteria && Criteria{SysUser::Cols::_sex,
                                 static_cast<int16_t>(*request.getSex())};
    }
    if (request.getDeptId())
    {
        criteria =
            criteria && Criteria{SysUser::Cols::_dept_id, *request.getDeptId()};
    }
    if (request.getPhoneNumber())
    {
        criteria = criteria && Criteria{SysUser::Cols::_phone_number,
                                        CompareOperator::Like,
                                        "%" + *request.getPhoneNumber() + "%"};
    }
    if (request.getEmail())
    {
        criteria = criteria && Criteria{SysUser::Cols::_email,
                                        CompareOperator::Like,
                                        "%" + *request.getEmail() + "%"};
    }
    if (request.getStatus())
    {
        criteria =
            criteria && Criteria{SysUser::Cols::_status,
                                 static_cast<int16_t>(*request.getStatus())};
    }
    return criteria;
}

vector<UserResponse> UserCqrsRepo::buildUserList(
    const vector<SysUser> &sysUserList) const
{
    vector<UserResponse> userList;
    for (const auto &sysUser : sysUserList)
    {
        userList.push_back(UserResponse{User{sysUser}});
    }
    return userList;
}

inline CoroMapper<SysUser> UserCqrsRepo::userMapper()
{
    static CoroMapper<SysUser> userMapper{app().getDbClient()};
    return userMapper;
}
