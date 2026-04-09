#include "RoleController.h"

#include <drogon/HttpTypes.h>
#include <drogon/utils/Utilities.h>

using namespace std;
using namespace drogon;
using namespace drogon::utils;

Task<HttpResponsePtr> RoleController::list(const HttpRequestPtr req,
                                           const string name,
                                           const string deptId,
                                           const string page,
                                           const string pageSize) const
{
    // TODO: 改成构造
    GetRoleListRequest request;
    if (name.size() > 0)
    {
        request.setName(name);
    }
    if (deptId.size() > 0)
    {
        try
        {
            const auto value = fromString<int32_t>(deptId);
            request.setDeptId(value);
        }
        catch (const exception & /* ignore */)
        {
        }
    }
    if (page.size() > 0)
    {
        try
        {
            const auto value = fromString<int32_t>(page);
            request.setPage(value);
        }
        catch (const exception & /* ignore */)
        {
            request.setPage(1);
        }
    }
    if (pageSize.size() > 0)
    {
        try
        {
            const auto value = fromString<int32_t>(pageSize);
            request.setPageSize(value);
        }
        catch (const exception & /* ignore */)
        {
            request.setPageSize(10);
        }
    }
    const auto paginated = co_await roleService_->getRoleList(request);
    co_return toResponse(paginated);
}

Task<HttpResponsePtr> RoleController::createRole(
    const HttpRequestPtr req,
    const RoleCreateRequest request) const
{
    const auto createdBy = fromString<int32_t>(req->getParameter("userId"));
    co_await roleService_->createRole(request, createdBy);
    co_return HttpResponse::newHttpResponse(k201Created, CT_NONE);
}

Task<HttpResponsePtr> RoleController::updateRole(
    const HttpRequestPtr req,
    const int32_t roleId,
    const RoleUpdateRequest request) const
{
    const auto updatedBy = fromString<int32_t>(req->getParameter("userId"));
    co_await roleService_->updateRole(roleId, request, updatedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

Task<HttpResponsePtr> RoleController::deleteRole(const HttpRequestPtr req,
                                                 const int32_t roleId) const
{
    const auto deletedBy = fromString<int32_t>(req->getParameter("userId"));
    co_await roleService_->deleteRole(roleId, deletedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

Task<HttpResponsePtr> RoleController::getAssignableRoles(
    const HttpRequestPtr req,
    const int32_t deptId) const
{
    co_return toResponse(co_await roleService_->getAssignableRoles(deptId));
}
