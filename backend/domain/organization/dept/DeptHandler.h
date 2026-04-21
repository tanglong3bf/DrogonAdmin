#pragma once

#include <cstdint>
#include <memory>
#include <drogon/utils/coroutine.h>
#include "common/framework/DrAdminObject.hpp"
#include "Dept.h"
#include "DeptVerifier.h"
#include "domain/organization/user/UserVerifier.h"
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
    drogon::Task<> deleteDept(Dept &dept, const std::int32_t deletedBy) const;

    /**
     * @brief 排序部门
     */
    drogon::Task<std::vector<Dept>> sortDept(
        const std::optional<std::int32_t> &parentId,
        const std::vector<int32_t> &deptIds,
        const int32_t updatedBy) const;

  private:
    /// @brief 验证部门id列表都在部门列表中
    void validateDeptIdsInAllDepts(const std::vector<std::int32_t> &deptIds,
                                   const std::vector<Dept> &allDepts) const;

  private:
    DeptRepositoryPtr deptRepository_{
        drogon::DrClassMap::getSingleInstance<DeptRepository>()};
    DeptVerifierPtr deptVerifier_{
        drogon::DrClassMap::getSingleInstance<DeptVerifier>()};
    UserVerifierPtr userVerifier_{
        drogon::DrClassMap::getSingleInstance<UserVerifier>()};
    // 跨模块
    RoleVerifierPtr roleVerifier_{
        drogon::DrClassMap::getSingleInstance<RoleVerifier>()};
};

using DeptHandlerPtr = std::shared_ptr<DeptHandler>;
