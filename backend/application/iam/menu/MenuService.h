#pragma once

#include "MenuAssembler.h"
#include "MenuCqrsRepo.h"
#include "MenuCreateRequest.h"
#include "domain/iam/menu/MenuRepository.h"
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

    /**
     * @brief 新增菜单项
     */
    drogon::Task<> createMenu(const MenuCreateRequest &request,
                              const std::int32_t createdBy) const;

  private:
    MenuCqrsRepoPtr menuCqrsRepo_{
        drogon::DrClassMap::getSingleInstance<MenuCqrsRepo>()};
    MenuAssemblerPtr menuAssembler_{
        drogon::DrClassMap::getSingleInstance<MenuAssembler>()};
    MenuRepositoryPtr menuRepository_{
        drogon::DrClassMap::getSingleInstance<MenuRepository>()};
};

using MenuServicePtr = std::shared_ptr<MenuService>;
