#pragma once

#include "UserUpdateRequest.h"
#include "application/organization/user/UserInfoUpdateRequest.h"
#include "domain/organization/user/User.h"
#include "domain/organization/dept/DeptVerifier.h"
#include "domain/authorization/RoleVerifier.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/utils/coroutine.h>

/**
 * @brief 用户更新服务
 */
class UserUpdater : public DrAdminObject<UserUpdater>
{
  public:
    /**
     * @brief 更新用户
     */
    drogon::Task<> updateUser(User &user,
                              const UserUpdateRequest &request,
                              const std::int32_t updatedBy) const;

    /**
     * @brief 更新用户基本信息
     */
    drogon::Task<> updateBasicInfo(User &user,
                                   const UserInfoUpdateRequest &request) const;

  protected:
    /**
     * @brief 更新用户和角色的关联关系
     */
    void updateUserRoles(std::vector<UserRole> &userRoles,
                         const std::vector<std::int32_t> &newRoleIds,
                         const std::int32_t userId,
                         const std::int32_t updatedBy) const;

  private:
    DeptVerifierPtr deptVerifier_{
        drogon::DrClassMap::getSingleInstance<DeptVerifier>()};
    RoleVerifierPtr roleVerifier_{
        drogon::DrClassMap::getSingleInstance<RoleVerifier>()};
};

using UserUpdaterPtr = std::shared_ptr<UserUpdater>;
