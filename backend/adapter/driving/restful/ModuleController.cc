#include "ModuleController.h"

using namespace drogon;

drogon::Task<drogon::HttpResponsePtr> ModuleController::getModuleTree(
    const drogon::HttpRequestPtr req) const
{
    const auto tree = co_await moduleService_->getModuleTree();
    co_return toResponse(tree);
}

drogon::Task<drogon::HttpResponsePtr> ModuleController::deleteModule(
    const drogon::HttpRequestPtr req,
    const std::int32_t moduleId) const
{
    const auto deletedBy = req->getAttributes()->get<int32_t>("userId");
    co_await moduleService_->deleteModule(moduleId, deletedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}
