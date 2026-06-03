#include "ChangeableEntity.h"

ChangingStatus ChangeableEntity::changingStatus() const noexcept
{
    return changingStatus_;
}

bool ChangeableEntity::isNew() const noexcept
{
    return changingStatus_ == ChangingStatus::NEW;
}

bool ChangeableEntity::isUpdated() const noexcept
{
    return changingStatus_ == ChangingStatus::UPDATED;
}

bool ChangeableEntity::isDeleted() const noexcept
{
    return changingStatus_ == ChangingStatus::DELETED;
}

void ChangeableEntity::markNew()
{
    changingStatus_ = ChangingStatus::NEW;
}

void ChangeableEntity::markUpdated()
{
    changingStatus_ = ChangingStatus::UPDATED;
}

void ChangeableEntity::markDeleted()
{
    changingStatus_ = ChangingStatus::DELETED;
}
