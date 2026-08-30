#include "ModuleController.h"

using namespace drogon;

Task<HttpResponsePtr> ModuleController::getModuleTree(
    const HttpRequestPtr req) const
{
    const auto tree = co_await moduleService_->getModuleTree();
    co_return toResponse(tree);
}

Task<HttpResponsePtr> ModuleController::createModule(
    const HttpRequestPtr req,
    const ModuleCreateRequest request) const
{
    const auto createdBy = req->getAttributes()->get<int32_t>("userId");
    co_await moduleService_->createModule(request, createdBy);
    co_return HttpResponse::newHttpResponse(k201Created, CT_NONE);
}

Task<HttpResponsePtr> ModuleController::updateModule(
    const HttpRequestPtr req,
    const std::int32_t moduleId,
    const ModuleUpdateRequest request) const
{
    const auto updatedBy = req->getAttributes()->get<int32_t>("userId");
    co_await moduleService_->updateModule(moduleId, request, updatedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

Task<HttpResponsePtr> ModuleController::deleteModule(
    const HttpRequestPtr req,
    const std::int32_t moduleId) const
{
    const auto deletedBy = req->getAttributes()->get<int32_t>("userId");

    const auto versionStr = req->getParameter("version");
    if (!isInteger(versionStr))
    {
        throw BusinessException("版本号必须为整数");
    }
    const int32_t version = stoi(versionStr);

    co_await moduleService_->deleteModule(moduleId, version, deletedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

Task<HttpResponsePtr> ModuleController::sortModule(
    const HttpRequestPtr req,
    const ModuleSortRequest request) const
{
    const auto updatedBy = req->getAttributes()->get<int32_t>("userId");
    co_await moduleService_->sortModule(request, updatedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

Task<HttpResponsePtr> ModuleController::updateActions(
    const HttpRequestPtr req,
    const int32_t moduleId,
    const ActionUpdateRequest request) const
{
    const auto updatedBy = req->getAttributes()->get<int32_t>("userId");
    co_await moduleService_->updateActions(moduleId, request, updatedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}
