#pragma once

#include <memory>
#include <drogon/utils/coroutine.h>
#include "common/framework/DrAdminObject.hpp"
#include "Dept.h"
#include "DeptVerifier.h"
#include "UserVerifier.h"
#include "domain/authorization/RoleVerifier.h"

/**
 * @brief 部门处理器
 */
class DeptHandler : public DrAdminObject<DeptHandler>
{
  public:
    /**
     * @brief 更新部门
     */
    drogon::Task<> updateDept(Dept &dept,
                              const std::string &newName,
                              const std::int32_t updatedBy) const;

    /**
     * @brief 删除部门
     */
    drogon::Task<> deleteDept(Dept &dept, const int32_t deletedBy) const;

    /**
     * @brief 排序部门
     */
    drogon::Task<std::vector<Dept>> sortDept(
        const std::vector<int32_t> &deptIds,
        const std::vector<Dept> &allDepts,
        const int32_t updatedBy) const;

  private:
    void validateNameNotSame(const std::string &oldName,
                             const std::string &newName) const;

    void validateDeptIdsInAllDepts(const std::vector<int32_t> &deptIds,
                                   const std::vector<Dept> &allDepts) const;

  private:
    DeptVerifierPtr deptVerifier_{
        drogon::DrClassMap::getSingleInstance<DeptVerifier>()};
    UserVerifierPtr userVerifier_{
        drogon::DrClassMap::getSingleInstance<UserVerifier>()};
    // 耦合
    RoleVerifierPtr roleVerifier_{
        drogon::DrClassMap::getSingleInstance<RoleVerifier>()};
};

using DeptHandlerPtr = std::shared_ptr<DeptHandler>;
