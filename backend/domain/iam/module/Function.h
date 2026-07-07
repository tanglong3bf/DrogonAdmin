#pragma once

#include "domain/models/SysFunction.h"
#include "common/framework/domain/AuditableEntity.h"
#include "common/framework/domain/ChangeableEntity.h"
#include <string>

class Function : public AuditableEntity, public ChangeableEntity
{
    using SysFunction = drogon_model::drogon_admin_db::SysFunction;
    friend class Module;
    friend class ModuleRepository;
    friend class ModuleCqrsRepo;

    /**
     * @brief 准备必备参数的构造
     */
    Function(std::string_view name,
             const std::string_view code,
             const std::int32_t sortNum,
             const std::int32_t moduleId);

    /**
     * @brief 准备必备参数以及创建者id的构造
     */
    Function(std::string_view name,
             const std::string_view code,
             const std::int32_t sortNum,
             const std::int32_t moduleId,
             const std::int32_t createdBy);

    /// @group 和model类互转
    /// @{
    explicit Function(const SysFunction &sysFunction);
    explicit operator SysFunction() const;
    /// @}

  public:
    GETTER(functionId);
    GETTER_STR_VIEW(name);
    GETTER_STR_VIEW(code);
    GETTER(description);
    GETTER(sortNum);
    GETTER(moduleId);

  private:
    std::optional<std::int32_t> functionId_;
    std::string name_;
    std::string code_;
    std::optional<std::string> description_;
    std::int32_t sortNum_;
    std::int32_t moduleId_;
};
