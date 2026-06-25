#include "UserController.h"

#include "application/organization/user/UserQueryRequest.h"
#include "common/util/ApiResponse.hpp"

using namespace std;
using namespace drogon;
using namespace drogon::utils;

// Add definition of your processing function here
Task<HttpResponsePtr> UserController::list(const HttpRequestPtr req,
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
    const auto &attr = req->getAttributes();
    UserQueryRequest request{username,
                             nickname,
                             sex,
                             deptId,
                             phoneNumber,
                             email,
                             status,
                             page,
                             pageSize,
                             attr};
    const auto paginated = co_await userService_->getUserList(request, attr);
    const auto response = ApiResponse{paginated, attr};
    co_return toResponse(response);
}

Task<HttpResponsePtr> UserController::createUser(
    const HttpRequestPtr req,
    const UserCreateRequest request) const
{
    const auto createdBy = req->getAttributes()->get<int32_t>("userId");
    co_await userService_->createUser(request, createdBy);
    co_return HttpResponse::newHttpResponse(k201Created, CT_NONE);
}

Task<HttpResponsePtr> UserController::updateUser(
    const HttpRequestPtr req,
    const std::int32_t userId,
    const UserUpdateRequest request) const
{
    const auto updatedBy = req->getAttributes()->get<int32_t>("userId");
    LOG_TRACE << "/user/" << userId << "(PUT)";
    LOG_TRACE << "更新用户，userId=" << userId << ", updatedBy=" << updatedBy;
    co_await userService_->updateUser(userId, request, updatedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

Task<HttpResponsePtr> UserController::deleteUser(
    const HttpRequestPtr req,
    const std::int32_t userId) const
{
    const auto deletedBy = req->getAttributes()->get<int32_t>("userId");
    co_await userService_->deleteUser(userId, deletedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

drogon::Task<drogon::HttpResponsePtr> UserController::updateBasicInfo(
    const drogon::HttpRequestPtr req,
    const UserInfoUpdateRequest request) const
{
    const auto userId = req->getAttributes()->get<int32_t>("userId");
    co_await userService_->updateBasicInfo(userId, request);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}
