#include "MenuVerifier.h"

#include "common/exception/BusinessException.h"

using namespace std;
using namespace drogon;
using namespace drogon::orm;

Task<> MenuVerifier::verifyMenuIsDirectory(const int32_t menuId) const
{
    const auto menu = co_await menuRepository_->getById(menuId);
    if (!menu)
    {
        throw BusinessException("指定的菜单id不存在");
    }
    if (!menu->isDirectory())
    {
        throw BusinessException("指定的菜单不是目录");
    }
}

Task<> MenuVerifier::verifyMenuNameNotDuplicated(
    const string &name,
    const optional<int32_t> &parentId) const
{
    const auto count =
        co_await menuRepository_->countNameByParentId(name, parentId);
    if (count > 0)
    {
        throw BusinessException("同级下已存在同名菜单");
    }
}

Task<> MenuVerifier::verifyActionNotUsed(
    const vector<std::int32_t> &actionIds) const
{
    const auto count = co_await menuRepository_->countByActionIds(actionIds);
    if (count > 0)
    {
        throw BusinessException("功能已被使用");
    }
}

Task<> MenuVerifier::verifyParentNotSelfOrDescendant(
    const int32_t menuId,
    const int32_t parentId) const
{
    if (co_await menuRepository_->isAncestorOrSelf(menuId, parentId))
    {
        throw BusinessException("不能将菜单移动到自身或其子菜单下");
    }
}
