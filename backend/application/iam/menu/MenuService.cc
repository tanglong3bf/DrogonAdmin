#include "MenuService.h"

#include "common/exception/BusinessException.h"
#include "common/util/rangesUtils.hpp"
#include <ranges>

using namespace std;
using namespace drogon;
using namespace drogon_admin;

Task<vector<MenuResponse>> MenuService::getMenuTree() const
{
    co_return co_await menuCqrsRepo_->getMenuTree();
}

Task<> MenuService::createMenu(const MenuCreateRequest &request,
                               const std::int32_t createdBy) const
{
    auto menu = co_await menuAssembler_->fromCreateRequest(request, createdBy);
    co_await menuRepository_->save(menu);
}

Task<> MenuService::updateMenu(const std::int32_t menuId,
                               const MenuUpdateRequest &request,
                               const std::int32_t updatedBy) const
{
    auto menu = co_await menuRepository_->getById(menuId);
    if (!menu)
    {
        throw BusinessException{"指定的菜单id不存在"};
    }
    co_await menuUpdater_->updateMenu(*menu, request, updatedBy);
    co_await menuRepository_->save(*menu);
}

Task<> MenuService::sortMenu(const MenuSortRequest &request,
                             const int32_t updatedBy) const
{
    vector<Menu> sortResult =
        co_await menuHandler_->sortMenu(request.parentId(),
                                        request.menuIds(),
                                        updatedBy);

    if (sortResult.size() == 0)
    {
        co_return;
    }

    const vector<int32_t> versions =
        sortResult |
        views::transform([](const auto &menu) { return *menu.menuId(); }) |
        views::transform([&request](const auto &menuId) {
            return ranges::find_if(request.menus(), [menuId](const auto &item) {
                return menuId == item.menuId;
            });
        }) |
        views::transform([](const auto &it) { return it->version; }) |
        ranges_utils::to<vector>();

    for (size_t i = 0; i < sortResult.size(); i++)
    {
        if (versions[i] != sortResult[i].version())
        {
            throw BusinessException("更新期间数据发生变化，更新失败");
        }
    }

    co_await menuRepository_->multiSave(sortResult, versions);
}
