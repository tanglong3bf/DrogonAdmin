#include "MenuController.h"

#include "common/exception/BusinessException.h"
#include <drogon/utils/Utilities.h>
#include <drogon/HttpTypes.h>

using namespace drogon;

Task<HttpResponsePtr> MenuController::getMenuTree(
    const HttpRequestPtr /*req*/) const
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
    const auto deletedBy = req->getAttributes()->get<int32_t>("userId");

    const auto versionStr = req->getParameter("version");
    if (!isInteger(versionStr))
    {
        throw BusinessException("版本号必须为整数");
    }
    const int32_t version = std::stoi(versionStr);

    co_await menuService_->deleteMenu(menuId, version, deletedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}

Task<HttpResponsePtr> MenuController::sortMenu(
    const HttpRequestPtr req,
    const MenuSortRequest request) const
{
    const auto updatedBy = req->getAttributes()->get<int32_t>("userId");
    co_await menuService_->sortMenu(request, updatedBy);
    co_return HttpResponse::newHttpResponse(k204NoContent, CT_NONE);
}
