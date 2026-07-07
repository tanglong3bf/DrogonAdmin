#pragma once

#include "Function.h"
#include "domain/models/SysModule.h"
#include "common/framework/domain/AuditableEntity.h"
#include "common/framework/domain/ChangeableEntity.h"
#include "common/util/Utilities.hpp"
#include <optional>
#include <string>
#include <cstdint>

/**
 * @brief 模块
 */
class Module : public AuditableEntity, public ChangeableEntity
{
    using SysModule = drogon_model::drogon_admin_db::SysModule;
    using SysFunction = drogon_model::drogon_admin_db::SysFunction;
    friend class ModuleRepository;

  public:
    /**
     * @brief 准备必备参数的构造
     */
    Module(std::string_view name, const std::int32_t sortNum);

    /**
     * @brief 准备必备参数以及创建者id的构造
     */
    Module(std::string_view name,
           const std::int32_t sortNum,
           const std::int32_t createdBy);

    /// @group 和model类互转
    /// @{
    explicit Module(const SysModule &sysModule);
    explicit operator SysModule() const;
    /// @}

    /**
     * @brief 标记删除
     */
    void remove(const int32_t deletedBy);

  public:
    GETTER(moduleId);
    GETTER_STR_VIEW(name);
    GETTER(description);
    GETTER(sortNum);
    GETTER(parentId);

    /**
     * @brief 追加功能（仅新增，不删除已有角色）
     * @param functions 待新增功能列表
     * @param createdBy 操作人ID，用于填充新建功能审计字段
     */
    void appendFunctions(std::vector<Function> &functions,
                         const int32_t createdBy);

    /**
     * @brief 差量对齐更新功能：保留交集、删除不在新列表的旧功能、新增缺少功能
     * @param newFunctions 最终需要持有的功能ID集合
     * @param updatedBy 本次更新操作人
     */
    void replaceFunctions(const std::vector<Function> &newFunctions,
                          const int32_t updatedBy);

    /**
     * @brief 仅在ModuleRepository中用于读取数据库数据
     */
    void restoreFunctions(const std::vector<SysFunction> &sysFunctions);

  private:
    std::optional<int32_t> moduleId_;         ///< 模块id
    std::string name_;                        ///< 模块名称
    std::optional<std::string> description_;  ///< 模块描述
    std::int32_t sortNum_;                    ///< 排序
    std::optional<int32_t> parentId_;         ///< 父模块
    std::vector<Function> functions_;         ///< 拥有的功能
};
