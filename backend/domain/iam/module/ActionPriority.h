#pragma once

#include "domain/models/SysActionPriority.h"
#include "common/util/Utilities.hpp"
#include "common/framework/domain/ChangeableEntity.h"

class ActionPriority : public ChangeableEntity
{
    using SysActionPriority = drogon_model::drogon_admin_db::SysActionPriority;

  public:
    ActionPriority(const std::int32_t highId,
                   const std::int32_t lowId,
                   const std::int32_t moduleId);

    ActionPriority(const std::int32_t highId,
                   const std::int32_t lowId,
                   const std::int32_t moduleId,
                   int32_t createdBy);

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
    std::int32_t highId_;        ///< 高优先级功能id
    std::int32_t lowId_;         ///< 低优先级功能id
    std::int32_t moduleId_;      ///< 所属模块id
    std::int32_t createdBy_;     ///< 创建者id
    trantor::Date createdTime_;  ///< 创建时间
};
