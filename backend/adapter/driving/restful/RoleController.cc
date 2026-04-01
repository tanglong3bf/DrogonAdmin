#include "RoleController.h"
#include <drogon/HttpTypes.h>
#include <drogon/utils/Utilities.h>

#include "common/exception/BusinessException.h"

using namespace drogon;

Task<HttpResponsePtr> RoleController::list(const drogon::HttpRequestPtr req,
                                           const std::string name,
                                           const std::string deptId,
                                           const std::string page,
                                           const std::string pageSize) const
{
    GetRoleListRequest request;
    if (name.size() > 0)
    {
        request.setName(name);
    }
    if (deptId.size() > 0)
    {
        try
        {
            const auto value = utils::fromString<int32_t>(deptId);
            request.setDeptId(value);
        }
        catch (const std::exception & /* ignore */)
        {
        }
    }
    if (page.size() > 0)
    {
        try
        {
            const auto value = utils::fromString<int32_t>(page);
            request.setPage(value);
        }
        catch (const std::exception & /* ignore */)
        {
            request.setPage(1);
        }
    }
    if (pageSize.size() > 0)
    {
        try
        {
            const auto value = utils::fromString<int32_t>(pageSize);
            request.setPageSize(value);
        }
        catch (const std::exception & /* ignore */)
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
    const auto createdBy =
        utils::fromString<int32_t>(req->getParameter("userId"));
    co_await roleService_->createRole(request, createdBy);
    co_return HttpResponse::newHttpResponse(k201Created, CT_NONE);
}

Task<HttpResponsePtr> RoleController::updateRole(
    const HttpRequestPtr req,
    const int32_t roleId,
    const RoleUpdateRequest request) const
{
    throw BusinessException("接口未实现");
}

Task<HttpResponsePtr> RoleController::deleteRole(const HttpRequestPtr req,
                                                 const int32_t roleId) const
{
    const auto deletedBy =
        utils::fromString<int32_t>(req->getParameter("userId"));
    co_await roleService_->deleteRole(roleId, deletedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}
