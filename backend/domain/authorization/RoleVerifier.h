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

  private:
    RoleRepositoryPtr roleRepository_{
        drogon::DrClassMap::getSingleInstance<RoleRepository>()};
};

using RoleVerifierPtr = std::shared_ptr<RoleVerifier>;
