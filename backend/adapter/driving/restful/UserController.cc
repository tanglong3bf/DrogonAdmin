#include "UserController.h"

#include "application/organization/user/UserQueryRequest.h"

using namespace drogon::utils;

// Add definition of your processing function here
drogon::Task<HttpResponsePtr> UserController::list(const HttpRequestPtr req,
                                                   const string username,
                                                   const string nickname,
                                                   const string sex,
                                                   const string deptId,
                                                   const string phoneNumber,
                                                   const string email,
                                                   const string status,
                                                   const string page,
                                                   const string pageSize) const
{
    UserQueryRequest request{username,
                             nickname,
                             sex,
                             deptId,
                             phoneNumber,
                             email,
                             status,
                             page,
                             pageSize};
    const auto paginated = co_await userService_->getUserList(request);
    co_return toResponse(paginated);
}

Task<HttpResponsePtr> UserController::createUser(
    const HttpRequestPtr req,
    const UserCreateRequest request) const
{
    const auto createdBy = fromString<int32_t>(req->getParameter("userId"));
    co_await userService_->createUser(request, createdBy);
    co_return HttpResponse::newHttpResponse(k201Created, CT_NONE);
}

drogon::Task<HttpResponsePtr> UserController::updateUser(
    const HttpRequestPtr req,
    const int32_t userId,
    const UserUpdateRequest request) const
{
    const auto updatedBy = fromString<int32_t>(req->getParameter("userId"));
    co_await userService_->updateUser(userId, request, updatedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}
