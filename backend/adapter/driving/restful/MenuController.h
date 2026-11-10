#pragma once

#include "application/iam/menu/MenuService.h"
#include <drogon/HttpController.h>

class MenuController : public drogon::HttpController<MenuController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(MenuController::getMenuTree,
                  "/menu/tree",
                  drogon::Get,
                  drogon::Options);
    ADD_METHOD_TO(MenuController::newMenu,
                  "/menu",
                  drogon::Post,
                  drogon::Options);
    ADD_METHOD_VIA_REGEX(MenuController::updateMenu,
                         "/menu/([1-9]\\d*)",
                         drogon::Patch,
                         drogon::Options);
    ADD_METHOD_VIA_REGEX(MenuController::deleteMenu,
                         "/menu/([1-9]\\d*)",
                         drogon::Delete,
                         drogon::Options);
    ADD_METHOD_TO(MenuController::sortMenu,
                  "/menu/sort",
                  drogon::Post,
                  drogon::Options);
    METHOD_LIST_END

    /**
     * @brief 获取目录树
     */
    drogon::Task<drogon::HttpResponsePtr> getMenuTree(
        const drogon::HttpRequestPtr req) const;

    /**
     * @brief 新增一个菜单项
     */
    drogon::Task<drogon::HttpResponsePtr> newMenu(
        const drogon::HttpRequestPtr req) const;

    /**
     * @brief 更新指定菜单项
     */
    drogon::Task<drogon::HttpResponsePtr> updateMenu(
        const drogon::HttpRequestPtr req,
        const std::int32_t menuId) const;

    /**
     * @brief 删除指定菜单项
     */
    drogon::Task<drogon::HttpResponsePtr> deleteMenu(
        const drogon::HttpRequestPtr req,
        const std::int32_t menuId) const;

    /**
     * @brief 排序菜单
     */
    drogon::Task<drogon::HttpResponsePtr> sortMenu(
        const drogon::HttpRequestPtr req) const;

  private:
    MenuServicePtr menuService_{
        drogon::DrClassMap::getSingleInstance<MenuService>()};
};
