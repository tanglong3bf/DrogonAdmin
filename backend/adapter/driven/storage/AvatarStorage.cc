#include "domain/org/user/AvatarStorage.h"

#include <drogon/drogon.h>
#include <fstream>

using namespace drogon;

Task<std::string> AvatarStorage::saveAvatar(const AvatarFileData &file) const
{
    const auto &config = drogon::app().getCustomConfig();
    const std::string imgPrefix =
        config.get("img_prefix", "/uploads/").asString();

    const std::string fileName = file.md5 + "." + std::string(file.extension);
    const std::string fullFilePath = app().getUploadPath() + "/" + fileName;

    std::ofstream outStream(fullFilePath, std::ios::binary | std::ios::trunc);
    if (!outStream.is_open())
    {
        throw std::runtime_error("无法创建头像文件: " + fullFilePath);
    }

    outStream.write(file.content.data(),
                    static_cast<std::streamsize>(file.content.size()));
    if (!outStream.good())
    {
        throw std::runtime_error("头像文件写入失败: " + fullFilePath);
    }
    outStream.close();

    co_return imgPrefix + fileName;
}
