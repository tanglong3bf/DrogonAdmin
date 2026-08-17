#include "DeptController.h"

#include <drogon/utils/Utilities.h>

using namespace drogon;

Task<HttpResponsePtr> DeptController::getDeptTree(
    const HttpRequestPtr req) const
{
    const auto tree = co_await deptService_->getDeptTree();
    co_return toResponse(tree);
}

Task<HttpResponsePtr> DeptController::createDept(
    const HttpRequestPtr req,
    const DeptCreateRequest request) const
{
    const auto createdBy = req->getAttributes()->get<int32_t>("userId");
    co_await deptService_->createDept(request, createdBy);
    co_return HttpResponse::newHttpResponse(k201Created, CT_NONE);
}

Task<HttpResponsePtr> DeptController::updateDept(
    const HttpRequestPtr req,
    const int32_t deptId,
    const DeptUpdateRequest request) const
{
    const auto updatedBy = req->getAttributes()->get<int32_t>("userId");
    co_await deptService_->updateDept(deptId, request, updatedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

Task<HttpResponsePtr> DeptController::deleteDept(const HttpRequestPtr req,
                                                 const int32_t deptId) const
{
    const auto deletedBy = req->getAttributes()->get<int32_t>("userId");

    const auto versionStr = req->getParameter("version");
    if (!isInteger(versionStr))
    {
        throw BusinessException("版本号必须为整数");
    }
    const int32_t version = stoi(versionStr);

    co_await deptService_->deleteDept(deptId, version, deletedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

Task<HttpResponsePtr> DeptController::sortDept(
    const HttpRequestPtr req,
    const DeptSortRequest request) const
{
    const auto updatedBy = req->getAttributes()->get<int32_t>("userId");
    co_await deptService_->sortDept(request, updatedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}
