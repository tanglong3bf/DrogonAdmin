#pragma once

#include <drogon/utils/coroutine.h>
#include "application/organization/user/UserUpdateRequest.h"
#include "common/framework/DrAdminObject.hpp"
#include "domain/organization/user/User.h"

class UserUpdater : public DrAdminObject<UserUpdater>
{
  public:
    /// 更新用户
    drogon::Task<> updateUser(User &user,
                              const UserUpdateRequest &request,
                              const std::int32_t updatedBy) const;

  protected:
    /// 更新用户和角色的关联关系
    void updateUserRoles(std::vector<UserRole> &userRoles,
                         const std::vector<std::int32_t> &newRoleIds,
                         const int32_t userId,
                         const int32_t updatedBy) const;
};

using UserUpdaterPtr = std::shared_ptr<UserUpdater>;
