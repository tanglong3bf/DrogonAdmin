#pragma once

#include <drogon/utils/coroutine.h>
#include <memory>
#include "application/organization/user/UserCqrsRepo.h"
#include "application/organization/user/UserQueryRequest.h"
#include "common/framework/DrAdminObject.hpp"
#include "common/util/PaginatedResponse.hpp"
#include "UserResponse.h"

/**
 * @brief 用户服务
 */
class UserService : public DrAdminObject<UserService>
{
  public:
    /**
     * @brief 带条件的分页查询用户列表
     */
    drogon::Task<PaginatedResponse<UserResponse>> getUserList(
        const UserQueryRequest &request) const;

  private:
    UserCqrsRepoPtr userCqrsRepo_{
        drogon::DrClassMap::getSingleInstance<UserCqrsRepo>()};
};

using UserServicePtr = std::shared_ptr<UserService>;
