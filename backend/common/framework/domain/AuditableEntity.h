#pragma once

#include "common/util/Utilities.hpp"
#include <cstdint>
#include <optional>
#include <trantor/utils/Date.h>

/**
 * @grief 可修改的实体类基类
 */
class AuditableEntity
{
  public:
    AuditableEntity(std::optional<std::uint32_t> createdBy = std::nullopt,
                    std::optional<trantor::Date> createdTime = std::nullopt,
                    std::optional<std::uint32_t> updatedBy = std::nullopt,
                    std::optional<trantor::Date> updatedTime = std::nullopt,
                    std::optional<std::uint32_t> deletedBy = std::nullopt,
                    std::optional<trantor::Date> deletedTime = std::nullopt);

    void markUpdatedBy(std::uint32_t updatedBy);

    void markDeletedBy(std::uint32_t deletedBy);

    // getters
    GETTER(createdBy)
    GETTER(createdTime)
    GETTER(updatedBy)
    GETTER(updatedTime)
    GETTER(deletedBy)
    GETTER(deletedTime)

  protected:
    std::optional<std::uint32_t> createdBy_;
    std::optional<trantor::Date> createdTime_;
    std::optional<std::uint32_t> updatedBy_;
    std::optional<trantor::Date> updatedTime_;
    std::optional<std::uint32_t> deletedBy_;
    std::optional<trantor::Date> deletedTime_;
};
