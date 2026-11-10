#include "application/iam/menu/MenuCqrsRepo.h"

#include <drogon/HttpAppFramework.h>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace tl::sql;
using namespace drogon_model::drogon_admin_db;

SqlGenerator *MenuCqrsRepo::sqlGenerator()
{
    static auto *plugin = app().getPlugin<SqlGenerator>();
    return plugin;
}

DbClientPtr MenuCqrsRepo::dbClient()
{
    static auto dbClient = app().getDbClient();
    return dbClient;
}

Task<vector<MenuResponse>> MenuCqrsRepo::getMenuTree() const
{
    const auto sql = sqlGenerator()->getSql("get_menu_tree");
    const auto dbResult = co_await dbClient()->execSqlCoro(sql);

    co_return buildTree(dbResult);
}

MenuResponse *MenuCqrsRepo::findById(const vector<MenuResponse> &tree,
                                     const int32_t menuId) const
{
    for (auto &item : tree)
    {
        if (item.menuId() == menuId)
        {
            return const_cast<MenuResponse *>(&item);
        }
        else if (item.children().size() > 0)
        {
            auto result = findById(item.children(), menuId);
            if (result != nullptr)
            {
                return result;
            }
        }
    }
    // getMenuTree应该保证parent先于child被查出，理论上不会走到这里
    return nullptr;
}

vector<MenuResponse> MenuCqrsRepo::buildTree(const Result &dbResult) const
{
    vector<MenuResponse> result;
    result.reserve(16);
    for (const auto &row : dbResult)
    {
        MenuResponse item{Menu{SysMenu{row}}};
        if (item.parentId())
        {
            auto *parent = findById(result, *item.parentId());
            parent->addChild(item);
        }
        else
        {
            result.emplace_back(item);
        }
    }
    return result;
}
