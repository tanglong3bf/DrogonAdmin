#include "UserCenterController.h"

using namespace drogon;

Task<HttpResponsePtr> UserCenterController::updateBasicInfo(
    const HttpRequestPtr req,
    const UserInfoUpdateRequest request) const
{
    const auto userId = req->getAttributes()->get<int32_t>("userId");
    co_await userCenterService_->updateBasicInfo(userId, request);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}
