#pragma once

#include <string>

struct AvatarFileData
{
    std::string content;    // 文件二进制内容
    std::string extension;  // 文件扩展名
    std::string md5;        // 文件MD5
};
