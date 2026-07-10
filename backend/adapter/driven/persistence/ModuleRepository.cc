#include "domain/iam/module/ModuleRepository.h"

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::drogon_admin_db;
using namespace tl::sql;

drogon::Task<> ModuleRepository::save(const Module &module,
                                      const DbClientPtr &dbClient) const
{
    switch (module.changingStatus())
    {
        case ChangingStatus::NEW:
            co_return;
        case ChangingStatus::DELETED:
        {
            auto trans = co_await dbClient->newTransactionCoro();
            auto mapper = moduleMapper(trans);

            const auto sysModule = static_cast<SysModule>(module);
            co_await mapper.update(sysModule);

            if (module.functions().size() > 0)
            {
                Json::Value data{Json::arrayValue};
                for (const auto func : module.functions())
                {
                    data.append(static_cast<SysFunction>(func).toJson());
                }

                const auto sql = sqlGenerator()->getSql("multi_update_function",
                                                        {{"data", data}});
                co_await trans->execSqlCoro(sql);
            }
            co_return;
        }
        case ChangingStatus::UPDATED:
            co_return;
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
            module.restoreFunctions(co_await functionMapper().findBy(criteria));
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

inline CoroMapper<SysFunction> ModuleRepository::functionMapper(
    const DbClientPtr &dbClient)
{
    return CoroMapper<SysFunction>{dbClient};
}
