#include "ModuleService.h"

using namespace std;
using namespace drogon;

Task<vector<ModuleResponse>> ModuleService::getModuleTree() const
{
    co_return co_await moduleCqrsRepo_->getModuleTree();
}

Task<> ModuleService::deleteModule(const std::int32_t moduleId,
                                   const std::int32_t deletedBy) const
{
    auto module = co_await moduleRepository_->getById(moduleId);
    if (!module)
    {
        // 没有数据，无需操作
        co_return;
    }
    co_await moduleHandler_->deleteModule(*module, deletedBy);
    co_await moduleRepository_->save(*module);
}
