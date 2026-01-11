#pragma once

#include <string>
#include "common/framework/domain/AuditableEntity.h"
#include "domain/models/SysDept.h"

/**
 * @brief 部门实体
 */
class Dept : public AuditableEntity
{
    // 新增时可能没有id
    std::optional<std::uint32_t> deptId_;    ///< 部门id
    std::string name_;                       ///< 部门名称
    std::uint32_t order_;                    ///< 排序
    std::optional<std::uint32_t> parentId_;  ///< 父部门id

  public:
    // 和model类互转
    Dept(const drogon_model::drogon_admin_db::SysDept &sysDept);
    operator drogon_model::drogon_admin_db::SysDept() const;

    // getters
    OPT_GETTER(deptId, DeptId)
    GETTER(name, Name)
    GETTER(order, Order)
    OPT_GETTER(parentId, ParentId)
};
