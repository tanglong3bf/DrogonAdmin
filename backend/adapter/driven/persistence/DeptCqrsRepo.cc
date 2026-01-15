#include "application/DeptCqrsRepo.h"

#include <drogon/HttpAppFramework.h>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace tl::sql;
using namespace drogon_model::drogon_admin_db;

SqlGenerator *DeptCqrsRepo::sqlGenerator()
{
    static auto *plugin = app().getPlugin<tl::sql::SqlGenerator>();
    return plugin;
}

DbClientPtr DeptCqrsRepo::dbClient()
{
    static auto dbClient = app().getDbClient();
    return dbClient;
}

Task<vector<DeptResponse>> DeptCqrsRepo::getDeptTree() const
{
    const auto sql = sqlGenerator()->getSql("get_dept_tree");
    const auto dbResult = co_await dbClient()->execSqlCoro(sql);

    co_return buildTree(dbResult);
}

DeptResponse *DeptCqrsRepo::findById(const vector<DeptResponse> &tree,
                                     const uint32_t deptId) const
{
    for (auto &item : tree)
    {
        if (item.getDeptId() == deptId)
        {
            return const_cast<DeptResponse *>(&item);
        }
        else if (item.getChildren().size() > 0)
        {
            auto result = findById(item.getChildren(), deptId);
            if (result != nullptr)
            {
                return result;
            }
        }
    }
    // 如果get_dept_tree没有问题，理论上不会走到这里
    return nullptr;
};

vector<DeptResponse> DeptCqrsRepo::buildTree(const Result &dbResult) const
{
    vector<DeptResponse> result;
    result.reserve(10);
    for (const auto &row : dbResult)
    {
        DeptResponse item{Dept{SysDept{row}}};
        if (item.getParentId())
        {
            auto *parent = findById(result, *item.getParentId());
            item.setParent(parent);
            parent->addChild(item);
        }
        else
        {
            result.emplace_back(item);
        }
    }
    return result;
}
