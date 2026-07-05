#include "domain/iam/module/ModuleRepository.h"

using namespace std;
using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::drogon_admin_db;
using namespace tl::sql;

drogon::Task<> ModuleRepository::save(const Module &module,
                                      const DbClientPtr &dbClient) const
{
    const auto sysModule = static_cast<SysModule>(module);

    auto mapper = moduleMapper(dbClient);

    switch (module.changingStatus())
    {
        case ChangingStatus::NEW:
            co_return;
        case ChangingStatus::DELETED:
        {
            co_await mapper.update(sysModule);
            // 完成功能管理时，删除该模块的所有功能
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
        co_return static_cast<Module>(sysModule);
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
