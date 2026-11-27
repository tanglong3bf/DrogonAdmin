#include "MenuController.h"

#include "common/exception/BusinessException.h"
#include <drogon/utils/Utilities.h>
#include <drogon/HttpTypes.h>

using namespace drogon;

Task<HttpResponsePtr> MenuController::getMenuTree(
    const HttpRequestPtr req) const
{
    const auto tree = co_await menuService_->getMenuTree();
    co_return toResponse(tree);
}

Task<HttpResponsePtr> MenuController::newMenu(
    const HttpRequestPtr req,
    const MenuCreateRequest request) const
{
    const auto createdBy = req->getAttributes()->get<int32_t>("userId");
    co_await menuService_->createMenu(request, createdBy);
    co_return HttpResponse::newHttpResponse(k201Created, CT_NONE);
}

Task<HttpResponsePtr> MenuController::updateMenu(
    const HttpRequestPtr req,
    const std::int32_t menuId,
    const MenuUpdateRequest request) const
{
    const auto updatedBy = req->getAttributes()->get<int32_t>("userId");
    co_await menuService_->updateMenu(menuId, request, updatedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

Task<HttpResponsePtr> MenuController::deleteMenu(
    const HttpRequestPtr req,
    const std::int32_t menuId) const
{
    throw BusinessException("接口未实现");
}

Task<HttpResponsePtr> MenuController::sortMenu(const HttpRequestPtr req) const
{
    throw BusinessException("接口未实现");
}
