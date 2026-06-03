#include "UserService.h"

using namespace drogon;

drogon::Task<PaginatedResponse<UserResponse>> UserService::getUserList(
    const UserQueryRequest &request) const
{
    const size_t count = co_await userCqrsRepo_->countByQueryReq(request);
    if (count == 0)
    {
        co_return PaginatedResponse<UserResponse>{1, request.pageSize(), 0, {}};
    }

    const size_t maxPage =
        (count + request.pageSize() - 1) / request.pageSize();
    const auto list = co_await userCqrsRepo_->getUserList(request, maxPage);

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
    auto user = co_await userRepository_->getById(userId, true);
    co_await userUpdater_->updateUser(user, request, updatedBy);
    co_await userRepository_->save(user);
}

drogon::Task<> UserService::deleteUser(const std::int32_t userId,
                                       const int32_t deletedBy) const
{
    auto user = co_await userRepository_->getById(userId, true);
    co_await userHandler_->deleteUser(user, deletedBy);
    co_await userRepository_->save(user);
}
