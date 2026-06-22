#include "UserService.h"

#include "common/exception/BusinessException.h"
#include <drogon/orm/Exception.h>

using namespace drogon;

drogon::Task<PaginatedResponse<UserResponse>> UserService::getUserList(
    const UserQueryRequest &request,
    const AttributesPtr &attr) const
{
    const size_t count = co_await userCqrsRepo_->countByQueryReq(request);
    if (count == 0)
    {
        co_return PaginatedResponse<UserResponse>{1, request.pageSize(), 0, {}};
    }

    const size_t maxPage =
        (count + request.pageSize() - 1) / request.pageSize();
    const auto list =
        co_await userCqrsRepo_->getUserList(request, maxPage, attr);

    co_return PaginatedResponse<UserResponse>{maxPage < request.page()
                                                  ? maxPage
                                                  : request.page(),
                                              request.pageSize(),
                                              count,
                                              list};
}

Task<> UserService::createUser(const UserCreateRequest &request,
                               const int32_t createdBy) const
{
    auto user = co_await userAssembler_->fromCreateRequest(request, createdBy);
    co_await userRepository_->save(user);
}

Task<> UserService::updateUser(const std::int32_t userId,
                               const UserUpdateRequest &request,
                               const int32_t updatedBy) const
{
    LOG_TRACE << "更新用户，userId=" << userId << ", updatedBy=" << updatedBy;
    try
    {
        auto user = co_await userRepository_->getById(userId, true);
        co_await userUpdater_->updateUser(user, request, updatedBy);
        co_await userRepository_->save(user);
    }
    catch (const orm::UnexpectedRows &e)
    {
        throw BusinessException("用户不存在");
    }
}

drogon::Task<> UserService::deleteUser(const std::int32_t userId,
                                       const int32_t deletedBy) const
{
    if (userId == 1)
    {
        throw BusinessException("id为1的用户不可以被删除");
    }
    try
    {
        auto user = co_await userRepository_->getById(userId, true);
        co_await userHandler_->deleteUser(user, deletedBy);
        co_await userRepository_->save(user);
    }
    catch (const orm::UnexpectedRows & /*ignore*/)
    {
        // 用户不存在
        co_return;
    }
}
