#include "ModuleVerifier.h"

#include "common/exception/BusinessException.h"

drogon::Task<> ModuleVerifier::verifyNoSubmodule(const int32_t moduleId) const
{
    const auto count = co_await moduleRepository_->countSubModule(moduleId);
    if (count > 0)
    {
        throw BusinessException("指定模块包含子模块");
    }
}

drogon::Task<> ModuleVerifier::verifyModuleExists(const int32_t moduleId) const
{
    const auto module = co_await moduleRepository_->getById(moduleId);
    if (!module)
    {
        throw BusinessException("指定的模块id不存在");
    }
}

drogon::Task<> ModuleVerifier::verifyModuleNameNotDuplicated(
    const std::string &name,
    const std::optional<int32_t> parentId) const
{
    const auto count =
        co_await moduleRepository_->countNameByParentId(name, parentId);
    if (count > 0)
    {
        throw BusinessException("模块名称冲突");
    }
}
