#pragma once

#include <memory>
#include <drogon/orm/CoroMapper.h>
#include <drogon/utils/coroutine.h>
#include "SqlGenerator/src/SqlGenerator.h"
#include "common/framework/DrAdminObject.hpp"
#include "domain/models/SysUser.h"
#include "domain/models/SysUserRole.h"
#include "User.h"

/**
 * @brief 用户仓库
 */
class UserRepository : public DrAdminObject<UserRepository>
{
    using DbClientPtr = drogon::orm::DbClientPtr;
    using SqlGenerator = tl::sql::SqlGenerator;
    using UserMapper =
        drogon::orm::CoroMapper<drogon_model::drogon_admin_db::SysUser>;
    using UserRoleMapper =
        drogon::orm::CoroMapper<drogon_model::drogon_admin_db::SysUserRole>;

  public:
    /**
     * @brief 统计指定部门有多少用户
     */
    drogon::Task<std::size_t> countByDept(const std::int32_t deptId) const;

    /**
     * @brief 统计指定角色被多少用户使用
     */
    drogon::Task<std::size_t> countByRole(const std::int32_t roleId) const;

    /**
     * @brief 统计指定用户名有多少用户
     */
    drogon::Task<std::size_t> countByUsername(
        const std::string &username) const;

    /**
     * @brief 统计指定昵称有多少用户
     */
    drogon::Task<std::size_t> countByNickname(
        const std::string &nickname) const;

    /**
     * @brief 保存用户
     */
    drogon::Task<> save(User &user) const;

  private:
    static SqlGenerator *sqlGenerator();
    static DbClientPtr dbClient();
    static UserMapper userMapper(
        const std::shared_ptr<drogon::orm::Transaction> trans = nullptr);
    static UserRoleMapper userRoleMapper();
};

using UserRepositoryPtr = std::shared_ptr<UserRepository>;
