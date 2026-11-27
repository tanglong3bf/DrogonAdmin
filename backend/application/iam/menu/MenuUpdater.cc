#include "MenuUpdater.h"

#include "common/exception/BusinessException.h"
#include "domain/models/SysMenu.h"

using namespace std;
using namespace drogon;

Task<> MenuUpdater::updateMenu(Menu &menu,
                               const MenuUpdateRequest &request,
                               const int32_t updatedBy) const
{
    assert(menu.menuId() != nullopt);

    if (menu.version() != request.version())
    {
        throw BusinessException{"更新期间数据发生变化，更新失败"};
    }

    // actionId 变化时，校验对应功能存在
    if (request.actionId() && *request.actionId() != menu.actionId())
    {
        co_await moduleVerifier_->verifyActionExists(*request.actionId());
    }

    // 计算更新后的父菜单
    optional<int32_t> newParentId = menu.parentId();
    if (request.parentId())
    {
        newParentId = *request.parentId();
    }
    else if (request.parentId().isNull())
    {
        newParentId = nullopt;
    }
    const bool parentChanged = newParentId != menu.parentId();

    int32_t sortNum = menu.sortNum();
    if (parentChanged)
    {
        if (newParentId)
        {
            co_await menuVerifier_->verifyMenuIsDirectory(*newParentId);
            co_await menuVerifier_->verifyParentNotSelfOrDescendant(
                *menu.menuId(), *newParentId);
        }

        // 移动父菜单后，放到新同级菜单的末尾
        const auto maxSortNum =
            co_await menuRepository_->getMaxSubMenuSortNum(newParentId);
        sortNum = maxSortNum ? *maxSortNum + 1 : 0;
    }

    // 名称或父菜单变化时，校验同级下名称唯一
    if ((request.name() && *request.name() != menu.name()) || parentChanged)
    {
        co_await menuVerifier_->verifyMenuNameNotDuplicated(
            request.name() ? *request.name() : menu.name(), newParentId);
    }

    menu.updateBasicInfo(request.name(),
                         request.parentId(),
                         sortNum,
                         request.icon(),
                         request.actionId(),
                         updatedBy);

    menu.updateTypeAndAccess(request.type(),
                             request.path(),
                             request.component(),
                             updatedBy);

    LOG_DEBUG << drogon_model::drogon_admin_db::SysMenu(menu)
                     .toJson()
                     .toStyledString();

    co_return;
}
