#pragma once

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

    // void appendAction(const std::vector<int32_t> &newActionIds,
    //                   const int32_t createdBy);
    // void replaceActions(const std::vector<int32_t> &actionIds,
    //                     const int32_t updatedBy);
    // void restoreActions(const std::vector<Xxx> &xxx);

  private:
    std::optional<int32_t> moduleId_;         ///< 模块id
    std::string name_;                        ///< 模块名称
    std::optional<std::string> description_;  ///< 模块描述
    std::int32_t sortNum_;                    ///< 排序
    std::optional<int32_t> parentId_;         ///< 父模块
    // std::vector<Action> actions_;             ///< 拥有的功能
};
