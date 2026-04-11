#pragma once

#include <memory>
#include <drogon/orm/CoroMapper.h>
#include <drogon/utils/coroutine.h>
#include "common/framework/DrAdminObject.hpp"
#include "SqlGenerator/src/SqlGenerator.h"
#include "domain/models/SysDept.h"
#include "Dept.h"

/**
 * @brief 部门仓库
 */
class DeptRepository : public DrAdminObject<DeptRepository>
{
    using DbClientPtr = drogon::orm::DbClientPtr;
    using SysDept = drogon_model::drogon_admin_db::SysDept;
    using SysDeptMapper = drogon::orm::CoroMapper<SysDept>;
    using SqlGenerator = tl::sql::SqlGenerator;

  public:
    /**
     * @brief 指定父部门id，获取最大的sort_num，传空表示根
     */
    drogon::Task<std::int32_t> getMaxSubDeptSortNum(
        const std::optional<std::int32_t> parentId) const;

    /**
     * @brief 存储部门数据
     */
    drogon::Task<> save(const Dept &dept) const;

    /**
     * @brief 根据部门id获取数据
     */
    drogon::Task<Dept> getById(const std::int32_t deptId) const;

    /**
     * @brief 统计指定父部门id下指定名称的数量
     */
    drogon::Task<std::int32_t> countNameByParentId(
        const std::string &name,
        const std::optional<std::int32_t> &parentId) const;

    /**
     * @brief 统计指定部门有多少个子部门
     */
    drogon::Task<std::size_t> countSubDept(const std::int32_t deptId) const;

    /**
     * @brief 批量获取部门
     */
    drogon::Task<std::vector<Dept>> getByIds(
        const std::vector<std::int32_t> &idsVector) const;

    /**
     * @brief 获取指定部门下的所有子部门
     */
    drogon::Task<std::vector<Dept>> getByParentId(
        const std::optional<std::int32_t> &parentId) const;

    /**
     * @brief 批量存储部门（新增、更新、删除）
     */
    drogon::Task<> multiSave(const std::vector<Dept> &depts) const;

  private:
    static SqlGenerator *sqlGenerator();
    static DbClientPtr dbClient();
    static SysDeptMapper deptMapper();
};

using DeptRepositoryPtr = std::shared_ptr<DeptRepository>;
