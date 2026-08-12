#include "domain/iam/module/ModuleRepository.h"

#include "common/util/rangesUtils.hpp"
#include <ranges>

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::drogon_admin_db;
using namespace tl::sql;
using namespace drogon_admin;

Task<> ModuleRepository::save(const Module &module,
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
                for (const auto action : module.actions())
                {
                    data.append(static_cast<SysAction>(action).toJson());
                }

                const auto sql = sqlGenerator()->getSql("multi_update_action",
                                                        {{"data", data}});
                co_await trans->execSqlCoro(sql);
            }
            if (module.priorities().size() > 0)
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

            // 采用分别存储的方式，是因为无法保证新增的actionId是有顺序的
            vector<int64_t> newActionIds;
            vector<int32_t> realActionIds;

            for (const auto item :
                 module.actions() | views::filter([](const Action &action) {
                     return action.actionId() > INT32_MAX;
                 }) | views::transform([](const Action &action) {
                     return action.actionId();
                 }))
            {
                newActionIds.emplace_back(item);
            }

            if (module.actions().size() > 0)
            {
                const auto toSysAction = [](const Action &action) {
                    return static_cast<SysAction>(action);
                };

                const auto pipeline = [&](ChangingStatus status) {
                    return module.actions() |
                           views::filter([status](const Action &action) {
                               return action.changingStatus() == status;
                           }) |
                           views::transform(toSysAction) |
                           ranges_utils::to<std::vector>();
                };

                const auto newActions = pipeline(ChangingStatus::NEW);
                const auto updatedActions = pipeline(ChangingStatus::UPDATED);
                const auto deletedActions = pipeline(ChangingStatus::DELETED);

                if (newActions.size() > 0)
                {
                    Json::Value data{Json::arrayValue};
                    for (const auto action : newActions)
                    {
                        data.append(action.toJson());
                    }
                    const auto sql =
                        sqlGenerator()->getSql("multi_insert_action",
                                               {{"data", data}});
                    const auto actionIds = co_await trans->execSqlCoro(sql);

                    for (size_t i = 0; i < actionIds.size(); ++i)
                    {
                        realActionIds.emplace_back(
                            actionIds[i][0].as<int32_t>());
                    }
                }
                if (updatedActions.size() > 0 || deletedActions.size() > 0)
                {
                    Json::Value data{Json::arrayValue};
                    for (const auto action : updatedActions)
                    {
                        data.append(action.toJson());
                    }
                    for (const auto action : deletedActions)
                    {
                        data.append(action.toJson());
                    }
                    const auto sql =
                        sqlGenerator()->getSql("multi_update_action",
                                               {{"data", data}});
                    co_await trans->execSqlCoro(sql);
                }
            }
            if (module.priorities().size() > 0)
            {
                const auto replacePriorityId =
                    [newActionIds,
                     realActionIds](const ActionPriority &priority) {
                        ActionPriority newPriority{priority};
                        if (newPriority.highId() > INT32_MAX)
                        {
                            const auto iter = ranges::find_if(
                                newActionIds, [newPriority](int64_t id) {
                                    return newPriority.highId() == id;
                                });
                            if (iter != newActionIds.end())
                            {
                                newPriority.highId_ =
                                    realActionIds[iter - newActionIds.begin()];
                            }
                        }
                        else if (newPriority.lowId() > INT32_MAX)
                        {
                            const auto iter = ranges::find_if(
                                newActionIds, [newPriority](int64_t id) {
                                    return newPriority.lowId() == id;
                                });
                            if (iter != newActionIds.end())
                            {
                                newPriority.lowId_ =
                                    realActionIds[iter - newActionIds.begin()];
                            }
                        }
                        return newPriority;
                    };

                const auto toSysPriority = [](const ActionPriority &priority) {
                    return static_cast<SysActionPriority>(priority);
                };

                const auto pipeline = [&](ChangingStatus status) {
                    return module.priorities() |
                           views::filter(
                               [status](const ActionPriority &priority) {
                                   return priority.changingStatus() == status;
                               }) |
                           views::transform(replacePriorityId) |
                           views::transform(toSysPriority) |
                           ranges_utils::to<std::vector>();
                };

                const auto newPriorities = pipeline(ChangingStatus::NEW);
                const auto deletedPriorities =
                    pipeline(ChangingStatus::DELETED);

                if (newPriorities.size() > 0)
                {
                    Json::Value data{Json::arrayValue};
                    for (const auto priority : newPriorities)
                    {
                        data.append(priority.toJson());
                    }
                    const auto sql =
                        sqlGenerator()->getSql("multi_insert_priority",
                                               {{"data", data}});
                    co_await trans->execSqlCoro(sql);
                }
                if (deletedPriorities.size() > 0)
                {
                    Json::Value data{Json::arrayValue};
                    for (const auto priority : deletedPriorities)
                    {
                        Json::Value item;
                        item["high_id"] = priority.getValueOfHighId();
                        item["low_id"] = priority.getValueOfLowId();
                        data.append(item);
                    }
                    const auto sql =
                        sqlGenerator()->getSql("multi_delete_priority",
                                               {{"data", data}});
                    co_await trans->execSqlCoro(sql);
                }
            }
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
            module.restorePriorities(co_await actionPriorityMapper().findBy(
                Criteria{SysModule::Cols::_module_id, moduleId}));
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

Task<> ModuleRepository::multiSave(const vector<Module> &modules,
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

Task<vector<Action>> ModuleRepository::getActionByIds(
    const vector<std::int32_t> &actionIds) const
{
    assert(actionIds.size() > 0);

    Criteria criteria{SysAction::Cols::_deleted_by, CompareOperator::IsNull};
    criteria =
        criteria &&
        Criteria{SysAction::Cols::_action_id, CompareOperator::In, actionIds};

    co_return co_await actionMapper().findBy(criteria) |
        views::transform([](const auto &data) { return Action{data}; }) |
        ranges_utils::to<vector>();
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
