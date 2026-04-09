#pragma once

#include <drogon/HttpController.h>
#include "application/authorization/role/RoleService.h"
#include "application/authorization/role/dto/RoleCreateRequest.h"
#include "application/authorization/role/dto/RoleUpdateRequest.h"

class RoleController : public drogon::HttpController<RoleController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(RoleController::list,
                  "/role?name={}&dept_id={}&page={}&page_size={}",
                  drogon::Get,
                  drogon::Options);
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
    ADD_METHOD_TO(RoleController::getAssignableRoles,
                  "/role/assignable?dept_id={}",
                  drogon::Get,
                  drogon::Options);
    METHOD_LIST_END

    /**
     * @brief 带条件分页查询角色列表
     *
     * @param name 角色名称（模糊查询、可选）
     * @param deptId 部门id（可选）
     * @param page 页码（默认为1）
     * @param pageSize 每页数量（默认为10）
     */
    drogon::Task<drogon::HttpResponsePtr> list(
        const drogon::HttpRequestPtr req,
        const std::string name,
        const std::string deptId,
        const std::string page,
        const std::string pageSize) const;

    /**
     * @brief 新增角色
     *
     * @see RoleCreateRequest
     */
    drogon::Task<drogon::HttpResponsePtr> createRole(
        const drogon::HttpRequestPtr req,
        const RoleCreateRequest request) const;

    /**
     * 更新角色
     */
    drogon::Task<drogon::HttpResponsePtr> updateRole(
        const drogon::HttpRequestPtr req,
        const std::int32_t roleId,
        const RoleUpdateRequest request) const;

    /**
     * @brief 删除角色
     *
     * @param roleId 待删除角色id
     */
    drogon::Task<drogon::HttpResponsePtr> deleteRole(
        const drogon::HttpRequestPtr req,
        const int32_t roleId) const;

    /**
     * @brief 获取指定部门可以分配的角色
     */
    drogon::Task<drogon::HttpResponsePtr> getAssignableRoles(
        const drogon::HttpRequestPtr req,
        const int32_t deptId) const;

  private:
    RoleServicePtr roleService_{
        drogon::DrClassMap::getSingleInstance<RoleService>()};
};
