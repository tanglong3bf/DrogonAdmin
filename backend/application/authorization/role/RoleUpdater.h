#pragma once

#include <drogon/utils/coroutine.h>
#include <memory>
#include "dto/RoleUpdateRequest.h"
#include "common/framework/DrAdminObject.hpp"
#include "domain/authorization/Role.h"

/**
 * @brief 角色更新器
 */
class RoleUpdater : public DrAdminObject<RoleUpdater>
{
  public:
    /// 更新角色
    drogon::Task<> updateRole(Role &role,
                              const RoleUpdateRequest &request,
                              const std::int32_t updatedBy) const;

  protected:
    /// 更新角色和部门的关联关系
    void updateRoleDepts(std::vector<RoleDept> &roleDepts,
                         const std::vector<std::int32_t> &newDeptIds,
                         const int32_t roleId,
                         const int32_t updatedBy) const;
};

using RoleUpdaterPtr = std::shared_ptr<RoleUpdater>;
