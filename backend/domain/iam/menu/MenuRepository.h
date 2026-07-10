#pragma once

#include "SqlGenerator/src/SqlGenerator.h"
#include "common/framework/DrAdminObject.hpp"
#include "domain/models/SysMenu.h"
#include <drogon/HttpAppFramework.h>
#include <drogon/utils/coroutine.h>

class MenuRepository : public DrAdminObject<MenuRepository>
{
    using SqlGenerator = tl::sql::SqlGenerator;
    using SysMenu = drogon_model::drogon_admin_db::SysMenu;
    using SysMenuMapper = drogon::orm::CoroMapper<SysMenu>;
    using DbClientPtr = drogon::orm::DbClientPtr;

  public:
    drogon::Task<std::size_t> countByFunctionIds(
        const std::vector<std::int32_t> &functionIds) const;

  private:
    static SqlGenerator *sqlGenerator();
    static SysMenuMapper menuMapper(
        const DbClientPtr &dbClient = drogon::app().getDbClient());
};

using MenuRepositoryPtr = std::shared_ptr<MenuRepository>;
