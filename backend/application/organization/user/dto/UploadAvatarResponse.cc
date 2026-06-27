#include "UploadAvatarResponse.h"
#include <drogon/HttpResponse.h>

namespace drogon
{
template <>
HttpResponsePtr toResponse(const UploadAvatarResponse &response)
{
    Json::Value json;
    json["data"] = response.toJson();
    return HttpResponse::newHttpJsonResponse(json);
}
};  // namespace drogon

Json::Value UploadAvatarResponse::toJson() const
{
    Json::Value json;
    json["file_path"] = static_cast<std::string>(filePath_);
    return json;
}
