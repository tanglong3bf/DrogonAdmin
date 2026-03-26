#include "RoleController.h"
#include "common/exception/BusinessException.h"

using namespace drogon;

Task<HttpResponsePtr> RoleController::list(
    const HttpRequestPtr req,
    const GetRoleListRequest request) const
{
    const auto paginated = co_await roleService_->getRoleList(request);
    co_return toResponse(paginated);
}

Task<HttpResponsePtr> RoleController::createRole(const HttpRequestPtr req) const
{
    throw BusinessException("接口未实现");
}

Task<HttpResponsePtr> RoleController::updateRole(const HttpRequestPtr req) const
{
    throw BusinessException("接口未实现");
}

Task<HttpResponsePtr> RoleController::deleteRole(const HttpRequestPtr req) const
{
    throw BusinessException("接口未实现");
}
