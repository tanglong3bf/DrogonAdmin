#pragma once

#include <memory>
#include <drogon/utils/coroutine.h>
#include "common/framework/DrAdminObject.hpp"
#include "RoleRepository.h"

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
     * @brief 验证指定部门是否可用所有角色
     */
    drogon::Task<> verifyRolesBelongToDept(
        const std::int32_t deptId,
        const std::vector<std::int32_t> &roleIds) const;

  private:
    RoleRepositoryPtr roleRepository_{
        drogon::DrClassMap::getSingleInstance<RoleRepository>()};
};

using RoleVerifierPtr = std::shared_ptr<RoleVerifier>;
