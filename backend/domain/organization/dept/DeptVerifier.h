#pragma once

#include "DeptRepository.h"
#include "domain/authorization/RoleVerifier.h"
#include "domain/organization/user/UserRepository.h"
#include "common/framework/DrAdminObject.hpp"
#include <memory>

/**
 * @brief 部门校验器
 */
class DeptVerifier : public DrAdminObject<DeptVerifier>
{
  public:
    /**
     * @brief 校验指定部门是否存在
     */
    drogon::Task<> verifyDepartmentExists(const std::int32_t deptId) const;

    /**
     * @brief 指定父部门id，校验部门名称是否可用，传空表示根
     */
    drogon::Task<> verifyDeptNameNotDuplicated(
        const std::string &name,
        const std::optional<std::int32_t> &parentId) const;

    /**
     * $brief 验证指定部门没有子部门
     */
    drogon::Task<> verifyNoSubDept(const std::int32_t deptId) const;

    /**
     * @brief 验证指定部门可以新增一个具有指定角色列表的用户
     */
    drogon::Task<> verifyRoleAssignmentAllowed(
        const std::int32_t deptId,
        const std::vector<std::int32_t> roleIds) const;

  private:
    DeptRepositoryPtr deptRepository_{
        drogon::DrClassMap::getSingleInstance<DeptRepository>()};
    UserRepositoryPtr userRepository_{
        drogon::DrClassMap::getSingleInstance<UserRepository>()};
    // 耦合
    RoleVerifierPtr roleVerifier_{
        drogon::DrClassMap::getSingleInstance<RoleVerifier>()};
    // 耦合
    RoleRepositoryPtr roleRepository_{
        drogon::DrClassMap::getSingleInstance<RoleRepository>()};
};

using DeptVerifierPtr = std::shared_ptr<DeptVerifier>;
