#include "ChangeableEntity.h"

ChangeableEntity::ChangeableEntity()
    : changingStatus_{ChangingStatus::UNCHANGED}
{
}

ChangingStatus ChangeableEntity::getChangingStatus() const
{
    return changingStatus_;
}

void ChangeableEntity::toUpdate()
{
    changingStatus_ = ChangingStatus::UPDATED;
}

void ChangeableEntity::toDelete()
{
    changingStatus_ = ChangingStatus::DELETED;
}

void ChangeableEntity::toUnChang()
{
    changingStatus_ = ChangingStatus::UNCHANGED;
}
