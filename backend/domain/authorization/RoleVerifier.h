#pragma once

#include <memory>
#include <drogon/utils/coroutine.h>
#include "common/framework/DrAdminObject.hpp"
#include "RoleRepository.h"
#include "domain/organization/user/UserRepository.h"

/**
 * @brief 角色校验器
 */
class RoleVerifier : public DrAdminObject<RoleVerifier>
{
  public:
    /**
     * @brief 验证指定部门不存在belong关系的角色
     */
    drogon::Task<> verifyNoRolesBelongToDept(const std::int32_t deptId) const;

    /**
     * @brief 验证角色名称是否重复
     */
    drogon::Task<> verifyRoleNameNotDuplicated(const std::string &name) const;

    /**
     * @brief 验证角色代码是否重复
     */
    drogon::Task<> verifyRoleCodeNotDuplicated(const std::string &code) const;

    /**
     * @brief 验证指定角色列表是否全部存在
     */
    drogon::Task<> verifyRolesExists(
        const std::vector<std::int32_t> &roleIds) const;
    /**
     * @brief 验证指定部门是否可为所有角色新增用户
     */
    drogon::Task<> verifyDeptRolesAllowedForNewUser(
        const std::int32_t oldDeptId,
        const std::int32_t newDeptId,
        const std::vector<std::int32_t> &allRoleIds,
        const std::vector<std::int32_t> &newRoleIds) const;

    /**
     * @brief 验证角色的用户数量限制是否满足要求
     */
    drogon::Task<> checkQuota(const Role &role, const Role &oldData) const;

  private:
    /**
     * @brief 验证指定部门是否可用所有角色
     */
    drogon::Task<> verifyRolesBelongToDept(
        const std::int32_t deptId,
        const std::vector<std::int32_t> &roleIds) const;

  private:
    RoleRepositoryPtr roleRepository_{
        drogon::DrClassMap::getSingleInstance<RoleRepository>()};
    UserRepositoryPtr userRepository_{
        drogon::DrClassMap::getSingleInstance<UserRepository>()};
};

using RoleVerifierPtr = std::shared_ptr<RoleVerifier>;
