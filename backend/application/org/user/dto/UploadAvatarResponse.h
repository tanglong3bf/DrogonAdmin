#pragma once

#include <json/value.h>
#include <string_view>

/**
 * @brief 上传头像响应
 */
struct UploadAvatarResponse
{
    explicit UploadAvatarResponse(std::string_view filePath)
        : filePath_{filePath}
    {
    }

    Json::Value toJson() const;

  private:
    std::string filePath_;  ///< 头像路径
};
