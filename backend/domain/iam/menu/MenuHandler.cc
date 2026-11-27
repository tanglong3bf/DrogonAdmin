#include "MenuHandler.h"

#include "common/exception/BusinessException.h"
#include <unordered_set>

using namespace std;
using namespace drogon;

Task<vector<Menu>> MenuHandler::sortMenu(const optional<std::int32_t> &parentId,
                                         const vector<int32_t> &menuIds,
                                         const int32_t updatedBy) const
{
    const auto allMenus = co_await menuRepository_->getByParentId(parentId);
    if (parentId)
    {
        const auto parent = co_await menuRepository_->getById(*parentId);
        if (!parent)
        {
            throw BusinessException{"父菜单不存在"};
        }
    }
    // 验证
    validateMenuIdsInAllMenus(menuIds, allMenus);
    if (allMenus.size() != menuIds.size())
    {
        throw BusinessException{"未指定全部菜单"};
    }

    // 菜单id到allMenus索引的映射
    unordered_map<int32_t, size_t> idToIndex;
    for (size_t i = 0; i < allMenus.size(); ++i)
    {
        auto id = *allMenus[i].menuId();
        idToIndex[id] = i;
    }

    // 哪些索引的菜单已经被处理
    vector<bool> processed(allMenus.size(), false);

    // 排序结果
    vector<Menu> sortResult;
    sortResult.reserve(allMenus.size());

    size_t menuProcessedCount = 0;

    for (int32_t id : menuIds)
    {
        const auto it = idToIndex.find(id);
        if (it != idToIndex.end())
        {
            const int32_t newSortNum = menuProcessedCount++;
            const size_t idx = it->second;
            if (newSortNum != allMenus[idx].sortNum())
            {
                Menu menu = allMenus[idx];
                menu.updateSortNum(newSortNum, updatedBy);
                sortResult.push_back(std::move(menu));
            }
            processed[idx] = true;
        }
    }
    co_return sortResult;
}

Task<> MenuHandler::deleteMenu(Menu &menu,
                               const int32_t version,
                               const int32_t deletedBy) const
{
    // 校验
    if (version != menu.version())
    {
        throw BusinessException{"删除期间数据发生变化，删除失败"};
    }
    co_await menuVerifier_->verifyNoSubMenu(*menu.menuId());

    menu.remove(deletedBy);
}

void MenuHandler::validateMenuIdsInAllMenus(const vector<int32_t> &menuIds,
                                            const vector<Menu> &allMenus) const
{
    unordered_set<int32_t> allMenuIds;
    for (const auto &menu : allMenus)
    {
        allMenuIds.insert(*menu.menuId());
    }

    for (const auto &menuId : menuIds)
    {
        if (allMenuIds.count(menuId) == 0)
        {
            throw BusinessException{"部分菜单不是指定菜单的子菜单"};
        }
    }
}
