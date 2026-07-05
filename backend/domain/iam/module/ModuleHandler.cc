#include "ModuleHandler.h"

drogon::Task<> ModuleHandler::deleteModule(Module &module,
                                           const int32_t deletedBy) const
{
    // 校验
    co_await moduleVerifier_->verifyNoSubmodule(*module.moduleId());
    // TODO: 校验当前模块和子模块的功能有没有被使用
    module.remove(deletedBy);
}
