#include "ModuleAssembler.h"

using namespace std;
using namespace drogon;
using namespace drogon::orm;

Task<Module> ModuleAssembler::fromCreateRequest(ModuleCreateRequest request,
                                                int32_t createdBy)
{
    // 验证
    if (request.parentId())
    {
        co_await moduleVerifier_->verifyModuleExists(*request.parentId());
    }
    co_await moduleVerifier_->verifyModuleNameNotDuplicated(string(
                                                                request.name()),
                                                            request.parentId());

    // 准备必备参数
    const auto maxSortNum =
        co_await moduleRepository_->getMaxSubModuleSortNum(request.parentId());

    Module module{string(request.name()),
                  maxSortNum ? *maxSortNum + 1 : 0,
                  createdBy};

    // 可选参数
    module.setParentId(request.parentId());

    co_return module;
}
