#pragma once

#include "Menu.h"
#include "MenuRepository.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/utils/coroutine.h>
#include <memory>
#include <cstdint>

/**
 * @brief 菜单处理器
 */
class MenuHandler : public DrAdminObject<MenuHandler>
{
  public:
    /**
     * @brief 排序菜单
     */
    drogon::Task<std::vector<Menu>> sortMenu(
        const std::optional<std::int32_t> &parentId,
        const std::vector<int32_t> &menuIds,
        const int32_t updatedBy) const;

  private:
    /// @brief 验证菜单id列表都在菜单列表中
    void validateMenuIdsInAllMenus(const std::vector<std::int32_t> &menuIds,
                                   const std::vector<Menu> &allMenus) const;

  private:
    MenuRepositoryPtr menuRepository_{
        drogon::DrClassMap::getSingleInstance<MenuRepository>()};
};

using MenuHandlerPtr = std::shared_ptr<MenuHandler>;
