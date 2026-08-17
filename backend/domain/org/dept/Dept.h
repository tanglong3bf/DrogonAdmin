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

    const auto &deptId() const noexcept
    {
        return deptId_;
    }
    GETTER(name)
    GETTER(sortNum)
    GETTER(parentId)
    GETTER(version)

    void setParentId(const std::optional<std::int32_t> parentId);

    /**
     * @brief 更新名称
     */
    void updateName(std::string_view name, const int32_t updatedBy);

    void updateSortNum(const std::int32_t sortNum, const std::int32_t updatedBy)
    {
        sortNum_ = sortNum;
        markUpdatedBy(updatedBy);
        markUpdated();
    }

    void remove(const std::int32_t deletedBy);

  private:
    // 新增时没有id
    std::optional<std::int32_t> deptId_;    ///< 部门id
    std::string name_;                      ///< 部门名称
    std::int32_t sortNum_;                  ///< 排序
    std::optional<std::int32_t> parentId_;  ///< 父部门id
    std::int32_t version_;                  ///< 乐观锁版本号
};
