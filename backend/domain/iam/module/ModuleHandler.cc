#include "ModuleHandler.h"

#include "common/util/rangesUtils.hpp"
#include <ranges>

using namespace std;

drogon::Task<> ModuleHandler::deleteModule(Module &module,
                                           const int32_t deletedBy) const
{
    // 校验
    // 检查没有子模块
    co_await moduleVerifier_->verifyNoSubmodule(*module.moduleId());

    const vector<std::int32_t> functionIds =
        module.functions() |
        views::transform([](const Function &f) { return *f.functionId(); }) |
        drogon_admin::ranges_utils::to<vector>();
    LOG_DEBUG << functionIds.size();

    if (functionIds.size() > 0)
    {
        // 检查模块的功能没有被分配权限
        co_await roleVerifier_->verifyFunctionNotUsed(functionIds);
        // 检查模块的功能没有被菜单使用
        co_await menuVerifier_->verifyFunctionNotUsed(functionIds);
    }

    // 标记删除
    module.remove(deletedBy);
}
