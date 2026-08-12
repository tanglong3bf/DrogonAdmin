#pragma once

#include <drogon/utils/coroutine.h>
#include "common/framework/DrAdminObject.hpp"

// 文件数据传输对象：与技术无关的纯数据结构
struct AvatarFileData
{
    std::string content;    // 文件二进制内容
    std::string extension;  // 文件扩展名
    std::string md5;        // 文件MD5（若业务要求MD5命名则保留）
};

class AvatarStorage : public DrAdminObject<AvatarStorage>
{
  public:
    virtual drogon::Task<std::string> saveAvatar(
        const AvatarFileData &file) const;
};

using AvatarStoragePtr = std::shared_ptr<AvatarStorage>;
