#pragma once

#include <memory>
#include "common/framework/DrAdminObject.hpp"
#include "DeptRepository.h"

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

  private:
    DeptRepositoryPtr deptRepository_{
        drogon::DrClassMap::getSingleInstance<DeptRepository>()};
};

using DeptVerifierPtr = std::shared_ptr<DeptVerifier>;
