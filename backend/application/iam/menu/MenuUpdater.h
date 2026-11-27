#pragma once

#include "MenuUpdateRequest.h"
#include "domain/iam/menu/Menu.h"
#include "domain/iam/menu/MenuRepository.h"
#include "domain/iam/menu/MenuVerifier.h"
#include "domain/iam/module/ModuleVerifier.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/utils/coroutine.h>

class MenuUpdater : public DrAdminObject<MenuUpdater>
{
  public:
    drogon::Task<> updateMenu(Menu &menu,
                              const MenuUpdateRequest &request,
                              std::int32_t updatedBy) const;

  private:
    MenuVerifierPtr menuVerifier_{
        drogon::DrClassMap::getSingleInstance<MenuVerifier>()};
    ModuleVerifierPtr moduleVerifier_{
        drogon::DrClassMap::getSingleInstance<ModuleVerifier>()};
    MenuRepositoryPtr menuRepository_{
        drogon::DrClassMap::getSingleInstance<MenuRepository>()};
};

using MenuUpdaterPtr = std::shared_ptr<MenuUpdater>;
