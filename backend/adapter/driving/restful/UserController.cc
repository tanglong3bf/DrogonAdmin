#include "UserController.h"

#include "application/organization/user/UserQueryRequest.h"

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
