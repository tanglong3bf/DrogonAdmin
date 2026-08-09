#pragma once

#include "domain/models/SysActionPriority.h"
#include "common/util/Utilities.hpp"
#include "common/framework/domain/ChangeableEntity.h"

class ActionPriority : public ChangeableEntity
{
    using SysActionPriority = drogon_model::drogon_admin_db::SysActionPriority;
    friend class ModuleRepository;

  public:
    ActionPriority(std::int64_t highId,
                   std::int64_t lowId,
                   std::int32_t moduleId);

    ActionPriority(std::int64_t highId,
                   std::int64_t lowId,
                   std::int32_t moduleId,
                   std::int32_t createdBy);

    /// @group 和model类互转
    /// @{
    explicit ActionPriority(const SysActionPriority &sysActionPriority);
    explicit operator SysActionPriority() const;
    /// @}

  public:
    GETTER(highId);
    GETTER(lowId);
    GETTER(moduleId);
    GETTER(createdBy);
    GETTER(createdTime);

  private:
    std::int64_t highId_;        ///< 高优先级功能id
    std::int64_t lowId_;         ///< 低优先级功能id
    std::int32_t moduleId_;      ///< 所属模块id
    std::int32_t createdBy_;     ///< 创建者id
    trantor::Date createdTime_;  ///< 创建时间
};
