#pragma once

#include <drogon/utils/coroutine.h>
#include "common/framework/DrAdminObject.hpp"

class AvatarStorage : public DrAdminObject<AvatarStorage>
{
  public:
    virtual std::string saveAvatar(std::string_view content,
                                   std::string_view extension,
                                   std::string_view md5) const;
};

using AvatarStoragePtr = std::shared_ptr<AvatarStorage>;
