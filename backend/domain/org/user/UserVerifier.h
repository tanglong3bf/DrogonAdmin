#pragma once

#include <memory>
#include <drogon/utils/coroutine.h>
#include "common/framework/DrAdminObject.hpp"
#include "UserRepository.h"

/**
 * @brief 用户校验器
 */
class UserVerifier : public DrAdminObject<UserVerifier>
{
  public:
    /**
     * 验证没有用户属于指定部门
     */
    drogon::Task<> verifyNoUserInDept(const std::int32_t deptId) const;

    /**
     * @brief 验证角色未被用户使用
     */
    drogon::Task<> verifyRoleNotUsed(const std::int32_t roleId) const;

    /**
     * @brief 用户名不重复
     */
    drogon::Task<> verifyUsernameNotDuplicated(
        const std::string &username) const;

    /**
     * @brief 昵称不重复
     */
    drogon::Task<> verifyNicknameNotDuplicated(
        const std::string &nickname) const;

    /**
     * @brief 验证被分配指定角色的用户都属于指定部门
     */
    drogon::Task<> verifyUsersWithRoleBelongToDepts(
        const std::int32_t roleId,
        const std::vector<int32_t> deptIds) const;

    /**
     * @brief 验证被分配指定角色的用户都不属于指定部门
     */
    drogon::Task<> verifyUsersWithRoleNotBelongToDepts(
        const std::int32_t roleId,
        const std::vector<int32_t> deptIds) const;
  private:
    UserRepositoryPtr userRepository_{
        drogon::DrClassMap::getSingleInstance<UserRepository>()};
};

using UserVerifierPtr = std::shared_ptr<UserVerifier>;
