#pragma once

#include <drogon/HttpController.h>
#include "application/authorization/GetRoleListRequest.h"
#include "application/authorization/RoleService.h"

class RoleController : public drogon::HttpController<RoleController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(RoleController::list, "/role", drogon::Get, drogon::Options);
    ADD_METHOD_TO(RoleController::createRole,
                  "/role",
                  drogon::Post,
                  drogon::Options);
    ADD_METHOD_VIA_REGEX(RoleController::updateRole,
                         "/role/([1-9]\\d*)",
                         drogon::Put,
                         drogon::Options);
    ADD_METHOD_VIA_REGEX(RoleController::deleteRole,
                         "/role/([1-9]\\d*)",
                         drogon::Delete,
                         drogon::Options);
    METHOD_LIST_END

    /**
     * 带条件分页查询角色列表
     */
    drogon::Task<drogon::HttpResponsePtr> list(
        const drogon::HttpRequestPtr req,
        const GetRoleListRequest request) const;

    /**
     * 新增角色
     */
    drogon::Task<drogon::HttpResponsePtr> createRole(
        const drogon::HttpRequestPtr req) const;

    /**
     * 更新角色
     */
    drogon::Task<drogon::HttpResponsePtr> updateRole(
        const drogon::HttpRequestPtr req) const;

    /**
     * 删除角色
     */
    drogon::Task<drogon::HttpResponsePtr> deleteRole(
        const drogon::HttpRequestPtr req) const;

  private:
    RoleServicePtr roleService_{
        drogon::DrClassMap::getSingleInstance<RoleService>()};
};
