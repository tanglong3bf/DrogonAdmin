#pragma once

#include <drogon/utils/coroutine.h>
#include "common/framework/DrAdminObject.hpp"
#include "domain/organization/user/User.h"

class UserHandler : public DrAdminObject<UserHandler>
{
  public:
    drogon::Task<> deleteUser(User &user, const std::int32_t deletedBy) const;
};

using UserHandlerPtr = std::shared_ptr<UserHandler>;
