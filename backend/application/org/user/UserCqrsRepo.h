#pragma once

#include "dto/UserQueryRequest.h"
#include "dto/UserResponse.h"
#include "domain/models/SysUser.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/orm/CoroMapper.h>
#include <drogon/utils/coroutine.h>

class UserCqrsRepo : public DrAdminObject<UserCqrsRepo>
{
    using SysUser = drogon_model::drogon_admin_db::SysUser;
    using UserMapper = drogon::orm::CoroMapper<SysUser>;
    using SysUserRole = drogon_model::drogon_admin_db::SysUserRole;
    using UserRoleMapper = drogon::orm::CoroMapper<SysUserRole>;

  public:
    /**
     * @brief 根据查询条件，统计数量
     */
    drogon::Task<std::size_t> countByQueryReq(
        const UserQueryRequest &request) const;

    /**
     * @brief 带条件分页查询
     */
    drogon::Task<std::vector<UserResponse>> getUserList(
        const UserQueryRequest &request,
        const int32_t maxPage,
        const drogon::AttributesPtr &attr) const;

  protected:
    drogon::orm::Criteria buildCriteria(const UserQueryRequest &request) const;
    std::vector<User> buildUserList(
        const std::vector<SysUser> &sysUserList) const;
    std::vector<UserResponse> buildUserResponseList(
        const std::vector<User> &userList) const;

  private:
    static UserMapper userMapper();
    static UserRoleMapper userRoleMapper();
};

using UserCqrsRepoPtr = std::shared_ptr<UserCqrsRepo>;
