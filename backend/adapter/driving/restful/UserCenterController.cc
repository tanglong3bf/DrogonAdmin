#include "UserCenterController.h"
#include <drogon/MultiPart.h>

using namespace drogon;

Task<HttpResponsePtr> UserCenterController::updateBasicInfo(
    const HttpRequestPtr req,
    const UserInfoUpdateRequest request) const
{
    const auto userId = req->getAttributes()->get<int32_t>("userId");
    co_await userCenterService_->updateBasicInfo(userId, request);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

Task<HttpResponsePtr> UserCenterController::changePassword(
    const HttpRequestPtr req,
    const ChangePasswordRequest request) const
{
    const auto userId = req->getAttributes()->get<int32_t>("userId");
    co_await userCenterService_->changePassword(userId, request);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

drogon::Task<drogon::HttpResponsePtr> UserCenterController::uploadAvatar(
    const drogon::HttpRequestPtr req) const
{
    const auto userId = req->getAttributes()->get<int32_t>("userId");
    const auto response =
        co_await userCenterService_->uploadAvatar(userId, req);
    co_return toResponse(response);
}
