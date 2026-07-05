#include "ModuleVerifier.h"

#include "common/exception/BusinessException.h"

drogon::Task<> ModuleVerifier::verifyNoSubmodule(const int32_t moduleId) const
{
    const auto count = co_await moduleRepository_->countSubModule(moduleId);
    if (count > 0)
    {
        throw BusinessException("指定部门包含子部门");
    }
}
