#pragma once

#include <drogon/utils/coroutine.h>
#include <memory>
#include "application/organization/user/UserAssembler.h"
#include "application/organization/user/UserCqrsRepo.h"
#include "application/organization/user/UserCreateRequest.h"
#include "application/organization/user/UserQueryRequest.h"
#include "application/organization/user/UserUpdateRequest.h"
#include "application/organization/user/UserUpdater.h"
#include "common/framework/DrAdminObject.hpp"
#include "common/util/PaginatedResponse.hpp"
#include "UserResponse.h"
#include "domain/organization/user/UserHandler.h"
#include "domain/organization/user/UserRepository.h"

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

    /**
     * @brief 新增用户
     */
    drogon::Task<> createUser(const UserCreateRequest &request,
                              const int32_t createdBy) const;

    /**
     * @brief 更新用户
     */
    drogon::Task<> updateUser(const std::int32_t userId,
                              const UserUpdateRequest &request,
                              const int32_t updatedBy) const;

    /**
     * @brief 删除用户
     */
    drogon::Task<> deleteUser(const std::int32_t userId,
                              const int32_t deletedBy) const;

  private:
    UserCqrsRepoPtr userCqrsRepo_{
        drogon::DrClassMap::getSingleInstance<UserCqrsRepo>()};
    UserAssemblerPtr userAssembler_{
        drogon::DrClassMap::getSingleInstance<UserAssembler>()};
    UserRepositoryPtr userRepository_{
        drogon::DrClassMap::getSingleInstance<UserRepository>()};
    UserUpdaterPtr userUpdater_{
        drogon::DrClassMap::getSingleInstance<UserUpdater>()};
    UserHandlerPtr userHandler_{
        drogon::DrClassMap::getSingleInstance<UserHandler>()};
};

using UserServicePtr = std::shared_ptr<UserService>;
