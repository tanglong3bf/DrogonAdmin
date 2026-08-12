#include "ModuleVerifier.h"

#include "common/exception/BusinessException.h"

using namespace std;
using namespace drogon;

Task<> ModuleVerifier::verifyNoSubmodule(const int32_t moduleId) const
{
    const auto count = co_await moduleRepository_->countSubModule(moduleId);
    if (count > 0)
    {
        throw BusinessException("指定模块包含子模块");
    }
}

Task<> ModuleVerifier::verifyModuleExists(const int32_t moduleId) const
{
    const auto module = co_await moduleRepository_->getById(moduleId);
    if (!module)
    {
        throw BusinessException("指定的模块id不存在");
    }
}

Task<> ModuleVerifier::verifyModuleNameNotDuplicated(
    const string &name,
    const optional<int32_t> parentId) const
{
    const auto count =
        co_await moduleRepository_->countNameByParentId(name, parentId);
    if (count > 0)
    {
        throw BusinessException("模块名称冲突");
    }
}

Task<> ModuleVerifier::verifyActionsBelongsToModule(
    const std::int32_t moduleId,
    const vector<std::int32_t> &actionIds) const
{
    assert(actionIds.size() > 0);

    const auto actions = co_await moduleRepository_->getActionByIds(actionIds);

    if (actions.size() != actionIds.size())
    {
        throw BusinessException("指定的功能id不存在");
    }
    for (const auto &action : actions)
    {
        if (action.moduleId() != moduleId)
        {
            throw BusinessException("指定的功能id不属于指定的模块");
        }
    }
}
