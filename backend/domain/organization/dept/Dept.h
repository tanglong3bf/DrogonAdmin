#pragma once

#include "domain/models/SysDept.h"
#include "common/framework/domain/AuditableEntity.h"
#include "common/framework/domain/ChangeableEntity.h"
#include <string>

/**
 * @brief 部门实体
 */
class Dept : public AuditableEntity, public ChangeableEntity
{
    using SysDept = drogon_model::drogon_admin_db::SysDept;

  public:
    /**
     * @brief 准备必备参数的构造
     */
    Dept(const std::string &name, const std::int32_t sortNum);

    /**
     * @brief 准备必备参数以及创建者id的构造
     */
    Dept(const std::string &name,
         const std::int32_t sortNum,
         const std::int32_t createdBy);

    /// @group 和model类互转
    /// @{
    explicit Dept(const SysDept &model);
    explicit operator SysDept() const;
    /// @}

  public:
    // 新增时没有id
    std::optional<std::int32_t> deptId;    ///< 部门id
    std::string name;                      ///< 部门名称
    std::int32_t sortNum;                  ///< 排序
    std::optional<std::int32_t> parentId;  ///< 父部门id
};
