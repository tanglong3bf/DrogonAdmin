#include "AuditableEntity.h"

AuditableEntity::AuditableEntity(std::optional<std::uint32_t> createdBy,
                                 std::optional<trantor::Date> createdTime,
                                 std::optional<std::uint32_t> updatedBy,
                                 std::optional<trantor::Date> updatedTime,
                                 std::optional<std::uint32_t> deletedBy,
                                 std::optional<trantor::Date> deletedTime)
    : createdBy_(createdBy),
      createdTime_(createdTime),
      updatedBy_(updatedBy),
      updatedTime_(updatedTime),
      deletedBy_(deletedBy),
      deletedTime_(deletedTime)
{
}

void AuditableEntity::setUpdatedBy(std::uint32_t updatedBy)
{
    updatedBy_ = updatedBy;
    updatedTime_ = trantor::Date::now();
}

void AuditableEntity::setDeletedBy(std::uint32_t deletedBy)
{
    deletedBy_ = deletedBy;
    deletedTime_ = trantor::Date::now();
}
