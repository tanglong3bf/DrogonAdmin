#include "MenuService.h"

#include "common/util/rangesUtils.hpp"

using namespace std;
using namespace drogon;

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
