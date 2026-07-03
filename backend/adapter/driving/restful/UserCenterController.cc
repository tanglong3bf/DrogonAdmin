#include "UserCenterController.h"
#include <drogon/MultiPart.h>
#include "domain/organization/user/AvatarStorage.h"

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
    MultiPartParser parser;
    parser.parse(req);
    const auto &files = parser.getFilesMap();

    if (!files.contains("avatar"))
    {
        throw BusinessException("缺少头像文件");
    }

    const HttpFile &avatarFile = files.at("avatar");
    AvatarFileData fileData{.content = static_cast<std::string>(
                                avatarFile.fileContent()),
                            .extension =
                                std::string(avatarFile.getFileExtension()),
                            .md5 = avatarFile.getMd5()};

    const auto response =
        co_await userCenterService_->uploadAvatar(userId, fileData);
    co_return toResponse(response);
}
