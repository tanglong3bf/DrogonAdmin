#include "UserCenterController.h"

using namespace std;
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

Task<HttpResponsePtr> UserCenterController::uploadAvatar(
    const HttpRequestPtr req) const
{
    const auto userId = req->getAttributes()->get<int32_t>("userId");

    MultiPartParser parser;
    parser.parse(req);
    const auto &files = parser.getFilesMap();

    if (!files.contains("avatar"))
    {
        throw BusinessException("缺少头像文件");
    }

    const HttpFile &avatarFile = files.at("avatar");

    string_view content = avatarFile.fileContent();
    string_view extension = avatarFile.getFileExtension();
    if (content.size() > 5 * 1024 * 1024)
    {
        throw BusinessException("头像文件过大");
    }

    if (extension != "jpg" && extension != "jpeg" && extension != "png")
    {
        throw BusinessException("不支持的图片格式，请改用jpg/jpeg/png");
    }

    const auto response =
        co_await userCenterService_->uploadAvatar(userId,
                                                  {.content = string(content),
                                                   .extension =
                                                       string(extension),
                                                   .md5 = avatarFile.getMd5()});
    co_return toResponse(response);
}
