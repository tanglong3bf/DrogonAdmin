#pragma once

#include "common/framework/domain/AuditableEntity.h"
#include "common/framework/domain/ChangeableEntity.h"

// 待定
enum class Scope : int16_t
{
    All = 0,
    CurrentDeptAndChildDepts = 1,
    CurrentDept = 2,
    CurrentUser = 3
};

/**
 * 权限
 */
class Permission : public AuditableEntity, public ChangeableEntity
{
  private:
    std::int32_t roleId_;
    std::int32_t actionId_;
    Scope scope_;
};
