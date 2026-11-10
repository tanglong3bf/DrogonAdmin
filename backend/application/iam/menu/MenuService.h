#pragma once

#include "application/iam/menu/MenuCqrsRepo.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/utils/coroutine.h>

/**
 * @brief 菜单服务
 */
class MenuService : public DrAdminObject<MenuService>
{
  public:
    MenuService() = default;

    /**
     * @brief 获取目录树
     */
    drogon::Task<std::vector<MenuResponse>> getMenuTree() const;

  private:
    MenuCqrsRepoPtr menuCqrsRepo_{
        drogon::DrClassMap::getSingleInstance<MenuCqrsRepo>()};
};

using MenuServicePtr = std::shared_ptr<MenuService>;
