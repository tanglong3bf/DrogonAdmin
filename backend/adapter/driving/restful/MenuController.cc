#include "MenuController.h"

#include <drogon/utils/Utilities.h>

#include "common/exception/BusinessException.h"

using namespace drogon;

Task<HttpResponsePtr> MenuController::getMenuTree(
    const HttpRequestPtr req) const
{
    const auto tree = co_await menuService_->getMenuTree();
    co_return toResponse(tree);
}

Task<HttpResponsePtr> MenuController::newMenu(const HttpRequestPtr req) const
{
    throw BusinessException("接口未实现");
}

Task<HttpResponsePtr> MenuController::updateMenu(
    const HttpRequestPtr req,
    const std::int32_t menuId) const
{
    throw BusinessException("接口未实现");
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
