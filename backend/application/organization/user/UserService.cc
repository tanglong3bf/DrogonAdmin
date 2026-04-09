#include "UserService.h"

using namespace drogon;

drogon::Task<PaginatedResponse<UserResponse>> UserService::getUserList(
    const UserQueryRequest &request) const
{
    const size_t count = co_await userCqrsRepo_->countByQueryReq(request);
    if (count == 0)
    {
        co_return PaginatedResponse<UserResponse>{1,
                                                  request.getPageSize(),
                                                  0,
                                                  {}};
    }

    const size_t maxPage =
        (count + request.getPageSize() - 1) / request.getPageSize();
    const auto list = co_await userCqrsRepo_->getUserList(request, maxPage);

    co_return PaginatedResponse<UserResponse>{maxPage < request.getPage()
                                                  ? maxPage
                                                  : request.getPage(),
                                              request.getPageSize(),
                                              count,
                                              list};
}

Task<> UserService::createUser(const UserCreateRequest &request,
                               const int32_t createdBy) const
{
    auto user = co_await userAssembler_->fromCreateRequest(request, createdBy);
    user.toNew();
    co_await userRepository_->save(user);
}
