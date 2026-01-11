#pragma once

#include <drogon/orm/DbClient.h>
#include <drogon/utils/coroutine.h>
#include <memory>
#include "DeptResponse.h"
#include "SqlGenerator/src/SqlGenerator.h"
#include "common/framework/DrAdminObject.hpp"

class DeptCqrsRepo : public DrAdminObject<DeptCqrsRepo>
{
  public:
    /**
     * @brief 获取部门树
     *
     * get_dept_tree应该保证查询结果的顺序，buildTree中不做检查
     */
    drogon::Task<std::vector<DeptResponse>> getDeptTree() const;

  protected:
    /**
     * @brief 在树中找到指定id的部门
     */
    DeptResponse *findById(const std::vector<DeptResponse> &tree,
                           const uint32_t deptId) const;

    /**
     * @brief 根据数据库查询结果构造一棵树
     */
    std::vector<DeptResponse> buildTree(
        const drogon::orm::Result &dbResult) const;

  private:
    static tl::sql::SqlGenerator *sqlGenerator();
    static drogon::orm::DbClientPtr dbClient();
};

using DeptCqrsRepoPtr = std::shared_ptr<DeptCqrsRepo>;
