#pragma once

#include "MenuRepository.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/utils/coroutine.h>

class MenuVerifier : public DrAdminObject<MenuVerifier>
{
  public:
    drogon::Task<> verifyActionNotUsed(
        const std::vector<std::int32_t> &actionIds) const;

  private:
    MenuRepositoryPtr menuRepository_{
        drogon::DrClassMap::getSingleInstance<MenuRepository>()};
};

using MenuVerifierPtr = std::shared_ptr<MenuVerifier>;
