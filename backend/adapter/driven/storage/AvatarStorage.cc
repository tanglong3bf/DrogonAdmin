#include "domain/org/user/AvatarStorage.h"

#include "common/exception/BusinessException.h"
#include <drogon/drogon.h>
#include <fstream>

using namespace std;
using namespace drogon;

string AvatarStorage::saveAvatar(string_view content,
                                 string_view extension,
                                 string_view md5) const
{
    const string fileName = format("{}.{}", md5, extension);
    const string fullFilePath =
        format("{}/{}", app().getUploadPath(), fileName);

    ofstream outStream{fullFilePath, ios::binary | ios::trunc};
    if (!outStream.is_open())
    {
        throw BusinessException{"无法创建头像文件: " + fullFilePath};
    }

    outStream.write(content.data(), static_cast<streamsize>(content.size()));
    if (!outStream)
    {
        throw BusinessException{"头像文件写入失败: " + fullFilePath};
    }

    return fileName;
}
