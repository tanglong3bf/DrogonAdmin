#pragma once

#include "UserAssembler.h"
#include "UserCqrsRepo.h"
#include "UserUpdater.h"
#include "dto/UserCreateRequest.h"
#include "dto/UserQueryRequest.h"
#include "dto/UserResponse.h"
#include "dto/UserUpdateRequest.h"
#include "domain/organization/user/UserRepository.h"
#include "common/framework/DrAdminObject.hpp"
#include "common/util/PaginatedResponse.hpp"
#include <drogon/utils/coroutine.h>
#include <memory>

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
        const UserQueryRequest &request,
        const drogon::AttributesPtr &attr) const;

    /**
     * @brief 新增用户
     */
    drogon::Task<> createUser(const UserCreateRequest &request,
                              const std::int32_t createdBy) const;

    /**
     * @brief 更新用户
     */
    drogon::Task<> updateUser(const std::int32_t userId,
                              const UserUpdateRequest &request,
                              const std::int32_t updatedBy) const;

    /**
     * @brief 删除用户
     */
    drogon::Task<> deleteUser(const std::int32_t userId,
                              const std::int32_t deletedBy) const;

  private:
    UserCqrsRepoPtr userCqrsRepo_{
        drogon::DrClassMap::getSingleInstance<UserCqrsRepo>()};
    UserAssemblerPtr userAssembler_{
        drogon::DrClassMap::getSingleInstance<UserAssembler>()};
    UserRepositoryPtr userRepository_{
        drogon::DrClassMap::getSingleInstance<UserRepository>()};
    UserUpdaterPtr userUpdater_{
        drogon::DrClassMap::getSingleInstance<UserUpdater>()};
};

using UserServicePtr = std::shared_ptr<UserService>;
