#pragma once

#include "domain/models/SysAction.h"
#include "common/framework/domain/AuditableEntity.h"
#include "common/framework/domain/ChangeableEntity.h"
#include <string>

class Action : public AuditableEntity, public ChangeableEntity
{
    using SysAction = drogon_model::drogon_admin_db::SysAction;
    friend class Module;
    friend class ModuleRepository;
    friend class ModuleCqrsRepo;
    friend class ModuleUpdater;

    /**
     * @brief 准备必备参数的构造
     */
    Action(std::int64_t actionId,
           std::string_view name,
           std::string_view code,
           std::int32_t sortNum,
           bool hasDataPermission,
           std::int32_t moduleId);

    /**
     * @brief 准备必备参数以及创建者id的构造
     */
    Action(std::int64_t actionId,
           std::string_view name,
           std::string_view code,
           std::int32_t sortNum,
           bool hasDataPermission,
           std::int32_t moduleId,
           std::int32_t createdBy);

    /// @group 和model类互转
    /// @{
    explicit Action(const SysAction &sysAction);
    explicit operator SysAction() const;
    /// @}

  public:
    GETTER(actionId);
    GETTER_STR_VIEW(name);
    GETTER_STR_VIEW(code);
    GETTER(description);
    GETTER(sortNum);
    GETTER(hasDataPermission);
    GETTER(moduleId);

    /**
     * @brief 更新Action信息
     */
    void updateInfo(std::string_view name,
                    std::string_view code,
                    std::int32_t sortNum,
                    bool hasDataPermission,
                    std::int32_t updatedBy);

  private:
    std::int64_t actionId_;
    std::string name_;
    std::string code_;
    std::optional<std::string> description_;
    std::int32_t sortNum_;
    bool hasDataPermission_;
    std::int32_t moduleId_;
};
