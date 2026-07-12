#include "ModuleUpdater.h"

drogon::Task<> ModuleUpdater::updateModule(Module &module,
                                           const ModuleUpdateRequest &request,
                                           const std::int32_t updatedBy)
{
    // 校验
    if (request.name() && request.name() != module.name())
    {
        co_await moduleVerifier_->verifyModuleNameNotDuplicated(
            *request.name(), module.parentId());
    }

    // 更新
    module.updateBasicInfo(request.name(), request.description(), updatedBy);
}
