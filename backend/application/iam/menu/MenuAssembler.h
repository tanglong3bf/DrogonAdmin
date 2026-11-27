#pragma once

#include "MenuCreateRequest.h"
#include "domain/iam/menu/Menu.h"
#include "domain/iam/menu/MenuRepository.h"
#include "domain/iam/menu/MenuVerifier.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/utils/coroutine.h>

/**
 * @brief 菜单装配器
 */
class MenuAssembler : public DrAdminObject<MenuAssembler>
{
  public:
    /**
     * @brief 用MenuCreateRequest构建Menu对象
     */
    drogon::Task<Menu> fromCreateRequest(const MenuCreateRequest &request,
                                         std::int32_t createdBy);

  private:
    MenuRepositoryPtr menuRepository_{
        drogon::DrClassMap::getSingleInstance<MenuRepository>()};
    MenuVerifierPtr menuVerifier_{
        drogon::DrClassMap::getSingleInstance<MenuVerifier>()};
};

using MenuAssemblerPtr = std::shared_ptr<MenuAssembler>;
