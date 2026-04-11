#pragma once

#include <drogon/utils/coroutine.h>
#include "common/framework/DrAdminObject.hpp"
#include "User.h"

class UserHandler : public DrAdminObject<UserHandler>
{
  public:
    /**
     * @brief 删除用户
     */
    drogon::Task<> deleteUser(User &user, const std::int32_t deletedBy) const;
};

using UserHandlerPtr = std::shared_ptr<UserHandler>;
