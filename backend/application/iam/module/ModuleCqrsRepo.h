#pragma once

#include "dto/ModuleResponse.h"
#include "common/framework/DrAdminObject.hpp"
#include "SqlGenerator/src/SqlGenerator.h"
#include <memory>

/**
 * @brief 模块数据仓库
 */
class ModuleCqrsRepo : public DrAdminObject<ModuleCqrsRepo>
{
  public:
    /**
     * @brief 获取模块树
     */
    drogon::Task<std::vector<ModuleResponse>> getModuleTree() const;

  protected:
    /**
     * @brief 在树中找到指定id的模块
     */
    ModuleResponse *findById(const std::vector<ModuleResponse> &tree,
                             const std::int32_t moduleId) const;

    /**
     * @brief 根据数据库查询结果构造一棵树
     */
    std::vector<ModuleResponse> buildTree(
        const drogon::orm::Result &dbResult) const;

  private:
    static tl::sql::SqlGenerator *sqlGenerator();
    static drogon::orm::DbClientPtr dbClient();
};

using ModuleCqrsRepoPtr = std::shared_ptr<ModuleCqrsRepo>;
