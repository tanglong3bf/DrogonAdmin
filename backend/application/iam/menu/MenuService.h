#pragma once

#include "MenuAssembler.h"
#include "MenuCqrsRepo.h"
#include "MenuCreateRequest.h"
#include "MenuSortRequest.h"
#include "MenuUpdateRequest.h"
#include "MenuUpdater.h"
#include "domain/iam/menu/MenuHandler.h"
#include "domain/iam/menu/MenuRepository.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/utils/coroutine.h>
#include <memory>

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

    /**
     * @brief 更新菜单项
     */
    drogon::Task<> updateMenu(const std::int32_t menuId,
                              const MenuUpdateRequest &request,
                              const std::int32_t updatedBy) const;

    /**
     * @brief 排序菜单
     */
    drogon::Task<> sortMenu(const MenuSortRequest &request,
                            const std::int32_t updatedBy) const;

  private:
    MenuCqrsRepoPtr menuCqrsRepo_{
        drogon::DrClassMap::getSingleInstance<MenuCqrsRepo>()};
    MenuAssemblerPtr menuAssembler_{
        drogon::DrClassMap::getSingleInstance<MenuAssembler>()};
    MenuUpdaterPtr menuUpdater_{
        drogon::DrClassMap::getSingleInstance<MenuUpdater>()};
    MenuHandlerPtr menuHandler_{
        drogon::DrClassMap::getSingleInstance<MenuHandler>()};
    MenuRepositoryPtr menuRepository_{
        drogon::DrClassMap::getSingleInstance<MenuRepository>()};
};

using MenuServicePtr = std::shared_ptr<MenuService>;
