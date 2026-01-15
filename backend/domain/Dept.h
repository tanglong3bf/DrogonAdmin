#pragma once

#include <string>
#include "common/framework/domain/AuditableEntity.h"
#include "common/framework/domain/ChangeableEntity.h"
#include "domain/models/SysDept.h"

/**
 * @brief 部门实体
 */
class Dept : public AuditableEntity, public ChangeableEntity
{
    using SysDept = drogon_model::drogon_admin_db::SysDept;

    // 新增时没有id
    std::optional<std::uint32_t> deptId_;    ///< 部门id
    std::string name_;                       ///< 部门名称
    std::uint32_t order_;                    ///< 排序
    std::optional<std::uint32_t> parentId_;  ///< 父部门id

  public:
    /**
     * @brief 准备必备参数的构造
     */
    Dept(const std::string &name, const std::uint32_t order);

    /**
     * @brief 准备必备参数以及创建者id的构造
     */
    Dept(const std::string &name,
         const std::uint32_t order,
         const std::uint32_t createdBy);

    // 和model类互转
    explicit Dept(const SysDept &sysDept);
    explicit operator SysDept() const;

    // setters
    SETTER(name, Name)
    OPT_SETTER(parentId, ParentId)

    // getters
    OPT_GETTER(deptId, DeptId)
    GETTER(name, Name)
    GETTER(order, Order)
    OPT_GETTER(parentId, ParentId)
};
