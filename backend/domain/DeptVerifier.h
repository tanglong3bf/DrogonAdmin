#pragma once

#include <memory>
#include "common/framework/DrAdminObject.hpp"
#include "domain/DeptRepository.h"

/**
 * @brief 部门校验器
 */
class DeptVerifier : public DrAdminObject<DeptVerifier>
{
  public:
    /**
     * @brief 校验指定部门是否存在
     */
    drogon::Task<> verifyDepartmentExists(const std::uint32_t deptId) const;

    /**
     * @brief 指定父部门id，校验部门名称是否可用，传空表示根
     */
    drogon::Task<> verifyDeptNameNotDuplicated(
        const std::string &name,
        const std::optional<std::uint32_t> &parentId) const;

  private:
    DeptRepositoryPtr deptRepository_{
        drogon::DrClassMap::getSingleInstance<DeptRepository>()};
};

using DeptVerifierPtr = std::shared_ptr<DeptVerifier>;
