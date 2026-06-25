#include "RoleController.h"

#include <drogon/HttpTypes.h>
#include <drogon/utils/Utilities.h>
#include "common/util/ApiResponse.hpp"

using namespace std;
using namespace drogon;
using namespace drogon::utils;

Task<HttpResponsePtr> RoleController::list(const HttpRequestPtr req,
                                           const string name,
                                           const string deptId,
                                           const string page,
                                           const string pageSize) const
{
    const auto &attr = req->getAttributes();
    RoleQueryRequest request{name, deptId, page, pageSize, attr};
    const auto paginated = co_await roleService_->getRoleList(request, attr);
    const auto response = ApiResponse(paginated, attr);
    co_return toResponse(response);
}

Task<HttpResponsePtr> RoleController::createRole(
    const HttpRequestPtr req,
    const RoleCreateRequest request) const
{
    const auto createdBy = req->getAttributes()->get<int32_t>("userId");
    co_await roleService_->createRole(request, createdBy);
    co_return HttpResponse::newHttpResponse(k201Created, CT_NONE);
}

Task<HttpResponsePtr> RoleController::updateRole(
    const HttpRequestPtr req,
    const std::int32_t roleId,
    const RoleUpdateRequest request) const
{
    const auto updatedBy = req->getAttributes()->get<int32_t>("userId");
    LOG_TRACE << "更新角色，roleId=" << roleId << ", updatedBy=" << updatedBy;
    co_await roleService_->updateRole(roleId, request, updatedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

Task<HttpResponsePtr> RoleController::deleteRole(
    const HttpRequestPtr req,
    const std::int32_t roleId) const
{
    const auto deletedBy = req->getAttributes()->get<int32_t>("userId");
    co_await roleService_->deleteRole(roleId, deletedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

Task<HttpResponsePtr> RoleController::getAssignableRoles(
    const HttpRequestPtr req,
    const std::int32_t deptId) const
{
    co_return toResponse(co_await roleService_->getAssignableRoles(deptId));
}
