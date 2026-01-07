#include "AuditableEntity.h"

AuditableEntity::AuditableEntity(std::optional<std::uint64_t> createdBy,
                                 std::optional<trantor::Date> createdTime,
                                 std::optional<std::uint64_t> updatedBy,
                                 std::optional<trantor::Date> updatedTime,
                                 std::optional<std::uint64_t> deletedBy,
                                 std::optional<trantor::Date> deletedTime)
    : createdBy_(createdBy),
      createdTime_(createdTime),
      updatedBy_(updatedBy),
      updatedTime_(updatedTime),
      deletedBy_(deletedBy),
      deletedTime_(deletedTime)
{
}

void AuditableEntity::setUpdatedBy(std::uint64_t updatedBy)
{
    updatedBy_ = updatedBy;
    updatedTime_ = trantor::Date::now();
}

void AuditableEntity::setDeletedBy(std::uint64_t deletedBy)
{
    deletedBy_ = deletedBy;
    deletedTime_ = trantor::Date::now();
}
