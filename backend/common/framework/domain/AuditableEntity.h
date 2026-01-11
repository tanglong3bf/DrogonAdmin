#pragma once

#include <trantor/utils/Date.h>
#include <cstdint>
#include <optional>

#include "common/util/Utilities.hpp"

/**
 * @grief 可修改的实体类基类
 */
class AuditableEntity
{
  protected:
    std::optional<std::uint32_t> createdBy_;
    std::optional<trantor::Date> createdTime_;
    std::optional<std::uint32_t> updatedBy_;
    std::optional<trantor::Date> updatedTime_;
    std::optional<std::uint32_t> deletedBy_;
    std::optional<trantor::Date> deletedTime_;

  public:
    AuditableEntity(std::optional<std::uint32_t> createdBy = std::nullopt,
                    std::optional<trantor::Date> createdTime = std::nullopt,
                    std::optional<std::uint32_t> updatedBy = std::nullopt,
                    std::optional<trantor::Date> updatedTime = std::nullopt,
                    std::optional<std::uint32_t> deletedBy = std::nullopt,
                    std::optional<trantor::Date> deletedTime = std::nullopt);

    void setUpdatedBy(std::uint32_t updatedBy);

    void setDeletedBy(std::uint32_t deletedBy);

    // getters
    OPT_GETTER(createdBy, CreatedBy)
    OPT_GETTER(createdTime, CreatedTime)
    OPT_GETTER(updatedBy, UpdatedBy)
    OPT_GETTER(updatedTime, UpdatedTime)
    OPT_GETTER(deletedBy, DeletedBy)
    OPT_GETTER(deletedTime, DeletedTime)
};
