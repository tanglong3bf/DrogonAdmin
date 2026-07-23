#include "application/iam/module/ModuleCqrsRepo.h"

#include "application/iam/module/ActionResponse.h"
#include "domain/models/SysAction.h"
#include <drogon/HttpAppFramework.h>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace tl::sql;
using namespace drogon_model::drogon_admin_db;

SqlGenerator *ModuleCqrsRepo::sqlGenerator()
{
    static auto *plugin = app().getPlugin<tl::sql::SqlGenerator>();
    return plugin;
}

DbClientPtr ModuleCqrsRepo::dbClient()
{
    static auto dbClient = app().getDbClient();
    return dbClient;
}

Task<vector<ModuleResponse>> ModuleCqrsRepo::getModuleTree() const
{
    const auto sql = sqlGenerator()->getSql("get_module_tree");
    const auto dbResult = co_await dbClient()->execSqlCoro(sql);

    co_return buildTree(dbResult);
}

ModuleResponse *ModuleCqrsRepo::findById(const vector<ModuleResponse> &tree,
                                         const std::int32_t moduleId) const
{
    for (auto &item : tree)
    {
        if (item.moduleId() == moduleId)
        {
            return const_cast<ModuleResponse *>(&item);
        }
        else if (item.children().size() > 0)
        {
            auto result = findById(item.children(), moduleId);
            if (result != nullptr)
            {
                return result;
            }
        }
    }
    // 如果get_module_tree没有问题，理论上不会走到这里
    return nullptr;
};

vector<ModuleResponse> ModuleCqrsRepo::buildTree(const Result &dbResult) const
{
    vector<ModuleResponse> result;
    for (const auto &row : dbResult)
    {
        ModuleResponse item{Module{SysModule{row}}};
        if (!row["action_list"].isNull())
        {
            for (const auto &actionRow : row["action_list"].as<Json::Value>())
            {
                ActionResponse action{Action{SysAction{actionRow}}};
                item.appendAction(action);
            }
        }
        if (!row["priority_list"].isNull())
        {
            for (const auto &priorityRow :
                 row["priority_list"].as<Json::Value>())
            {
                ActionPriorityResponse priority{
                    ActionPriority{SysActionPriority{priorityRow}}};
                item.appendPriority(priority);
            }
        }
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
