#include "application/organization/user/UserCqrsRepo.h"

#include "domain/organization/user/UserRole.h"
#include "domain/models/SysUser.h"
#include "common/util/AttrUtils.hpp"
#include <drogon/HttpAppFramework.h>
#include <drogon/orm/Criteria.h>

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
    const std::int32_t maxPage,
    const AttributesPtr &attr) const
{
    Criteria criteria = buildCriteria(request);

    const std::int32_t page =
        maxPage < request.page() ? maxPage : request.page();
    if (page != request.page())
    {
        addWarn(attr, "查询页码超出范围，已自动调整到最后一页");
    }
    const auto sysUserList = co_await userMapper()
                                 .orderBy(SysUser::Cols::_user_id)
                                 .paginate(page, request.pageSize())
                                 .findBy(criteria);
    auto userList = buildUserList(sysUserList);
    // 获取用户id列表
    vector<std::int32_t> userIdList{};
    for (const auto &user : userList)
    {
        userIdList.push_back(*user.userId());
    }

    auto userRolesList = co_await userRoleMapper().findBy(
        Criteria{SysUserRole::Cols::_user_id, CompareOperator::In, userIdList});
    for (auto &user : userList)
    {
        for (const auto &userRole : userRolesList)
        {
            if (userRole.getValueOfUserId() == user.userId())
            {
                user.addUserRole(UserRole{userRole});
            }
        }
    }
    co_return buildUserResponseList(userList);
}

Criteria UserCqrsRepo::buildCriteria(const UserQueryRequest &request) const
{
    Criteria criteria{SysUser::Cols::_deleted_by, CompareOperator::IsNull};
    if (request.username())
    {
        criteria = criteria && Criteria{SysUser::Cols::_username,
                                        CompareOperator::Like,
                                        "%" + *request.username() + "%"};
    }
    if (request.nickname())
    {
        criteria = criteria && Criteria{SysUser::Cols::_nickname,
                                        CompareOperator::Like,
                                        "%" + *request.nickname() + "%"};
    }
    if (request.sex())
    {
        criteria = criteria && Criteria{SysUser::Cols::_sex,
                                        static_cast<int16_t>(*request.sex())};
    }
    if (request.deptId())
    {
        criteria =
            criteria && Criteria{SysUser::Cols::_dept_id, *request.deptId()};
    }
    if (request.phoneNumber())
    {
        criteria = criteria && Criteria{SysUser::Cols::_phone_number,
                                        CompareOperator::Like,
                                        "%" + *request.phoneNumber() + "%"};
    }
    if (request.email())
    {
        criteria = criteria && Criteria{SysUser::Cols::_email,
                                        CompareOperator::Like,
                                        "%" + *request.email() + "%"};
    }
    if (request.status())
    {
        criteria =
            criteria && Criteria{SysUser::Cols::_status,
                                 static_cast<int16_t>(*request.status())};
    }
    return criteria;
}

vector<User> UserCqrsRepo::buildUserList(
    const vector<SysUser> &sysUserList) const
{
    vector<User> userList;
    for (const auto &sysUser : sysUserList)
    {
        userList.push_back(User{sysUser});
    }
    return userList;
}

vector<UserResponse> UserCqrsRepo::buildUserResponseList(
    const vector<User> &userList) const
{
    vector<UserResponse> userResponseList;
    for (const auto &user : userList)
    {
        userResponseList.push_back(UserResponse{user});
    }
    return userResponseList;
}

inline CoroMapper<SysUser> UserCqrsRepo::userMapper()
{
    static CoroMapper<SysUser> userMapper{app().getDbClient()};
    return userMapper;
}

inline CoroMapper<SysUserRole> UserCqrsRepo::userRoleMapper()
{
    static CoroMapper<SysUserRole> userRoleMapper{app().getDbClient()};
    return userRoleMapper;
}
