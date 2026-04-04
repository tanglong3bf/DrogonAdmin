#pragma once

#include <drogon/orm/CoroMapper.h>
#include <drogon/utils/coroutine.h>
#include "application/organization/user/UserQueryRequest.h"
#include "application/organization/user/UserResponse.h"
#include "common/framework/DrAdminObject.hpp"
#include "domain/models/SysUser.h"

class UserCqrsRepo : public DrAdminObject<UserCqrsRepo>
{
    using SysUser = drogon_model::drogon_admin_db::SysUser;
    using UserMapper = drogon::orm::CoroMapper<SysUser>;

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
        const int32_t maxPage) const;

  protected:
    drogon::orm::Criteria buildCriteria(const UserQueryRequest &request) const;
    std::vector<UserResponse> buildUserList(
        const std::vector<SysUser> &sysUserList) const;

  private:
    static UserMapper userMapper();
};

using UserCqrsRepoPtr = std::shared_ptr<UserCqrsRepo>;
