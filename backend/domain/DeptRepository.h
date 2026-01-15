#pragma once

#include <memory>
#include <drogon/orm/CoroMapper.h>
#include <drogon/utils/coroutine.h>
#include "common/framework/DrAdminObject.hpp"
#include "SqlGenerator/src/SqlGenerator.h"
#include "domain/Dept.h"
#include "domain/models/SysDept.h"

/**
 * @brief 部门仓库
 */
class DeptRepository : public DrAdminObject<DeptRepository>
{
    using DbClientPtr = drogon::orm::DbClientPtr;
    using SysDeptMapper =
        drogon::orm::CoroMapper<drogon_model::drogon_admin_db::SysDept>;
    using SqlGenerator = tl::sql::SqlGenerator;

  public:
    /**
     * @brief 指定父部门id，获取最大的order_no，传空表示根
     */
    drogon::Task<std::uint32_t> getMaxSubDeptOrder(
        const std::optional<std::uint32_t> parentId) const;

    /**
     * @brief 存储部门数据
     */
    drogon::Task<> save(const Dept &dept) const;

    /**
     * @brief 根据部门id获取数据
     */
    drogon::Task<Dept> getById(const std::uint32_t deptId) const;

    /**
     * @brief 统计指定父部门id下指定名称的数量
     */
    drogon::Task<std::uint32_t> countNameByParentId(
        const std::string &name,
        const std::optional<std::uint32_t> &parentId) const;

  protected:
    static SqlGenerator *sqlGenerator();
    static DbClientPtr dbClient();
    SysDeptMapper deptMapper() const;
};

using DeptRepositoryPtr = std::shared_ptr<DeptRepository>;
