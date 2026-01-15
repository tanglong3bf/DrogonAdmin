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
    const auto createdBy =
        utils::fromString<uint32_t>(req->getParameter("userId"));
    co_await deptService_->createDept(request, createdBy);
    co_return HttpResponse::newHttpResponse(k201Created, CT_NONE);
}

drogon::Task<drogon::HttpResponsePtr> DeptController::updateDept(
    const drogon::HttpRequestPtr req,
    const std::uint32_t deptId,
    const DeptUpdateRequest request) const
{
    const auto updatedBy =
        utils::fromString<uint32_t>(req->getParameter("userId"));
    co_await deptService_->updateDept(deptId, request, updatedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}
