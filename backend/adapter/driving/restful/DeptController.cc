#include "DeptController.h"

using namespace drogon;

Task<HttpResponsePtr> DeptController::getDeptTree(
    const HttpRequestPtr req) const
{
    const auto tree = co_await deptService_->getDeptTree();
    co_return toResponse(tree);
}

drogon::Task<drogon::HttpResponsePtr> DeptController::createDept(
    const drogon::HttpRequestPtr req,
    const DeptCreateRequest request) const
{
    const auto createdBy = req->getAttributes()->get<int32_t>("userId");
    co_await deptService_->createDept(request, createdBy);
    co_return HttpResponse::newHttpResponse(k201Created, CT_NONE);
}

drogon::Task<drogon::HttpResponsePtr> DeptController::updateDept(
    const drogon::HttpRequestPtr req,
    const std::int32_t deptId,
    const DeptUpdateRequest request) const
{
    const auto updatedBy = req->getAttributes()->get<int32_t>("userId");
    co_await deptService_->updateDept(deptId, request, updatedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

drogon::Task<drogon::HttpResponsePtr> DeptController::deleteDept(
    const drogon::HttpRequestPtr req,
    const std::int32_t deptId) const
{
    const auto deletedBy = req->getAttributes()->get<int32_t>("userId");
    co_await deptService_->deleteDept(deptId, deletedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

drogon::Task<drogon::HttpResponsePtr> DeptController::sortDept(
    const drogon::HttpRequestPtr req,
    const DeptSortRequest request) const
{
    const auto updatedBy = req->getAttributes()->get<int32_t>("userId");
    co_await deptService_->sortDept(request, updatedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}
