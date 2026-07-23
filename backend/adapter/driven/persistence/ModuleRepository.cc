#include "domain/iam/module/ModuleRepository.h"

#include "common/util/rangesUtils.hpp"
#include <ranges>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::drogon_admin_db;
using namespace tl::sql;
using namespace drogon_admin;

drogon::Task<> ModuleRepository::save(const Module &module,
                                      const DbClientPtr &dbClient) const
{
    switch (module.changingStatus())
    {
        case ChangingStatus::NEW:
            co_await moduleMapper(dbClient).insert(
                static_cast<SysModule>(module));
            co_return;
        case ChangingStatus::DELETED:
        {
            auto trans = co_await dbClient->newTransactionCoro();
            auto mapper = moduleMapper(trans);

            const auto sysModule = static_cast<SysModule>(module);
            co_await mapper.update(sysModule);

            if (module.actions().size() > 0)
            {
                Json::Value data{Json::arrayValue};
                for (const auto func : module.actions())
                {
                    data.append(static_cast<SysAction>(func).toJson());
                }

                const auto sql = sqlGenerator()->getSql("multi_update_action",
                                                        {{"data", data}});
                co_await trans->execSqlCoro(sql);
            }
            if (module.actionPriorities().size() > 0)
            {
                co_await actionPriorityMapper(trans).deleteBy(
                    Criteria{SysActionPriority::Cols::_module_id,
                             module.moduleId()});
            }
            co_return;
        }
        case ChangingStatus::UPDATED:
        {
            auto trans = co_await dbClient->newTransactionCoro();
            auto mapper = moduleMapper(trans);

            const auto sysModule = static_cast<SysModule>(module);
            co_await mapper.update(sysModule);
            co_return;
        }
        case ChangingStatus::UNCHANGED:
            co_return;
    }
}

Task<optional<Module>> ModuleRepository::getById(const std::int32_t moduleId,
                                                 const bool withRelation) const
{
    Criteria criteria{SysModule::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysModule::Cols::_module_id, moduleId};

    try
    {
        const auto sysModule = co_await moduleMapper().findOne(criteria);
        auto module = static_cast<Module>(sysModule);
        if (withRelation)
        {
            module.restoreActions(co_await actionMapper().findBy(criteria));
        }
        co_return module;
    }
    catch (const UnexpectedRows & /*ignore*/)
    {
        co_return nullopt;
    }
}

Task<size_t> ModuleRepository::countSubModule(const std::int32_t moduleId) const
{
    Criteria criteria{SysModule::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysModule::Cols::_parent_id, moduleId};

    co_return co_await moduleMapper().count(criteria);
}

Task<std::int32_t> ModuleRepository::countNameByParentId(
    const string &name,
    const optional<std::int32_t> &parentId) const
{
    Criteria criteria{SysModule::Cols::_deleted_by, CompareOperator::IsNull};
    criteria = criteria && Criteria{SysModule::Cols::_name, name};
    if (parentId)
    {
        criteria = criteria && Criteria{SysModule::Cols::_parent_id, parentId};
    }
    else
    {
        criteria = criteria && Criteria{SysModule::Cols::_parent_id,
                                        CompareOperator::IsNull};
    }

    co_return co_await moduleMapper().count(criteria);
}

Task<optional<std::int32_t>> ModuleRepository::getMaxSubModuleSortNum(
    const optional<std::int32_t> parentId,
    const DbClientPtr &dbClient) const
{
    ParamList paramList;
    if (parentId)
    {
        paramList = {{"parent_id", *parentId}};
    }

    const auto sql =
        sqlGenerator()->getSql("get_max_sub_module_sort_num", paramList);

    const auto dbResult = co_await dbClient->execSqlCoro(sql);
    if (dbResult[0][0].isNull())
    {
        co_return std::nullopt;
    }
    co_return dbResult[0][0].as<std::int32_t>();
}

Task<vector<Module>> ModuleRepository::getByParentId(
    const optional<int32_t> &parentId) const
{
    Criteria criteria{SysModule::Cols::_deleted_by, CompareOperator::IsNull};
    criteria =
        criteria && (parentId ? Criteria{SysModule::Cols::_parent_id, *parentId}
                              : Criteria{SysModule::Cols::_parent_id,
                                         CompareOperator::IsNull});
    const auto moduleInDb = co_await moduleMapper().findBy(criteria);
    co_return moduleInDb |
        views::transform([](const auto &data) { return Module{data}; }) |
        ranges_utils::to<vector>();
}

drogon::Task<> ModuleRepository::multiSave(const std::vector<Module> &modules,
                                           const DbClientPtr &dbClient) const
{
    // 暂只考虑批量更新
    vector<SysModule> toUpdate;
    toUpdate.reserve(modules.size());
    for (const auto &module : modules)
    {
        if (module.changingStatus() == ChangingStatus::UPDATED)
        {
            toUpdate.push_back(static_cast<SysModule>(module));
        }
    }
    if (toUpdate.size() == 0)
    {
        LOG_WARN << "multiSave被调用，但没有需要更新的数据，请检查代码逻辑";
        co_return;
    }
    Json::Value dataList;

    for (const auto &module : toUpdate)
    {
        Json::Value item;
        item = module.toJson();
        // SqlGenerator 太垃圾了
        for (const auto &key : item.getMemberNames())
        {
            if (item[key].isNull())
            {
                item.removeMember(key);
            }
        }
        dataList.append(item);
    }

    const auto sql = sqlGenerator()->getSql("multi_update_module",
                                            ParamList{{"data_list", dataList}});

    co_await dbClient->execSqlCoro(sql);
}

inline SqlGenerator *ModuleRepository::sqlGenerator()
{
    static const auto sqlGenerator_ = app().getPlugin<SqlGenerator>();
    return sqlGenerator_;
}

inline CoroMapper<SysModule> ModuleRepository::moduleMapper(
    const DbClientPtr &dbClient)
{
    return CoroMapper<SysModule>{dbClient};
}

inline CoroMapper<SysAction> ModuleRepository::actionMapper(
    const DbClientPtr &dbClient)
{
    return CoroMapper<SysAction>{dbClient};
}

inline CoroMapper<SysActionPriority> ModuleRepository::actionPriorityMapper(
    const DbClientPtr &dbClient)
{
    return CoroMapper<SysActionPriority>{dbClient};
}
