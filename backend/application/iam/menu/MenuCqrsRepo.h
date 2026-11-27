#pragma once

#include "MenuResponse.h"
#include "common/framework/DrAdminObject.hpp"
#include "SqlGenerator/src/SqlGenerator.h"
#include <drogon/orm/DbClient.h>
#include <drogon/utils/coroutine.h>
#include <memory>

/**
 * @brief 菜单数据仓库（CQRS读端）
 */
class MenuCqrsRepo : public DrAdminObject<MenuCqrsRepo>
{
  public:
    /**
     * @brief 获取目录树
     *
     * getMenuTree应该保证查询结果的顺序，buildTree中不做检查
     */
    drogon::Task<std::vector<MenuResponse>> getMenuTree() const;

  protected:
    /**
     * @brief 在树中找到指定id的菜单
     */
    MenuResponse *findById(const std::vector<MenuResponse> &tree,
                           const std::int32_t menuId) const;

    /**
     * @brief 根据数据库查询结果构造一棵树
     */
    std::vector<MenuResponse> buildTree(
        const drogon::orm::Result &dbResult) const;

  private:
    static tl::sql::SqlGenerator *sqlGenerator();
    static drogon::orm::DbClientPtr dbClient();
};

using MenuCqrsRepoPtr = std::shared_ptr<MenuCqrsRepo>;
