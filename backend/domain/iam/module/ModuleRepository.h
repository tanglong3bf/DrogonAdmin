#pragma once

#include "Module.h"
#include "common/framework/DrAdminObject.hpp"
#include "SqlGenerator/src/SqlGenerator.h"
#include <drogon/HttpAppFramework.h>
#include <drogon/utils/coroutine.h>

/**
 * @brief 模块数据仓库
 */
class ModuleRepository : public DrAdminObject<ModuleRepository>
{
    using DbClientPtr = drogon::orm::DbClientPtr;
    using SqlGenerator = tl::sql::SqlGenerator;
    using SysModule = drogon_model::drogon_admin_db::SysModule;
    using SysModuleMapper = drogon::orm::CoroMapper<SysModule>;
    using SysAction = drogon_model::drogon_admin_db::SysAction;
    using SysActionMapper = drogon::orm::CoroMapper<SysAction>;
    using SysActionPriority = drogon_model::drogon_admin_db::SysActionPriority;
    using SysActionPriorityMapper = drogon::orm::CoroMapper<SysActionPriority>;

  public:
    /**
     * @brief 存储模块数据
     */
    drogon::Task<> save(
        const Module &module,
        const DbClientPtr & = drogon::app().getDbClient()) const;

    /**
     * @brief 根据模块id获取数据
     */
    drogon::Task<std::optional<Module>> getById(
        const std::int32_t moduleId,
        const bool withRelation = false) const;

    /**
     * @brief 获取指定模块的子模块数量
     */
    drogon::Task<std::size_t> countSubModule(const std::int32_t moduleId) const;

    /**
     * @brief 指定父模块id，获取最大的sort_num，传空表示根
     */
    drogon::Task<std::optional<std::int32_t>> getMaxSubModuleSortNum(
        const std::optional<std::int32_t> parentId,
        const DbClientPtr & = drogon::app().getDbClient()) const;

    /**
     * @brief 统计指定父模块id下指定名称的数量
     */
    drogon::Task<std::int32_t> countNameByParentId(
        const std::string &name,
        const std::optional<std::int32_t> &parentId) const;

    /**
     * @brief 获取指定模块下的所有子模块
     */
    drogon::Task<std::vector<Module>> getByParentId(
        const std::optional<std::int32_t> &parentId) const;

    /**
     * @brief 批量存储模块（新增、更新、删除）
     */
    drogon::Task<> multiSave(
        const std::vector<Module> &modules,
        const std::vector<std::int32_t> &versions,
        const DbClientPtr & = drogon::app().getDbClient()) const;

    /**
     * @brief 根据id批量获取功能
     */
    drogon::Task<std::vector<Action>> getActionByIds(
        const std::vector<std::int32_t> &actionIds) const;

  private:
    static SqlGenerator *sqlGenerator();
    static SysModuleMapper moduleMapper(
        const DbClientPtr &dbClient = drogon::app().getDbClient());
    static SysActionMapper actionMapper(
        const DbClientPtr &dbClient = drogon::app().getDbClient());
    static SysActionPriorityMapper actionPriorityMapper(
        const DbClientPtr &dbClient = drogon::app().getDbClient());
};

using ModuleRepositoryPtr = std::shared_ptr<ModuleRepository>;
