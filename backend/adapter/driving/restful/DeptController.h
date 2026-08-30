#pragma once

#include "application/org/dept/DeptService.h"
#include "application/org/dept/DeptCreateRequest.h"
#include "application/org/dept/DeptSortRequest.h"
#include "application/org/dept/DeptUpdateRequest.h"
#include <drogon/HttpController.h>
#include <drogon/DrClassMap.h>

class DeptController : public drogon::HttpController<DeptController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(DeptController::getDeptTree,
                  "/dept/tree",
                  drogon::Get,
                  drogon::Options);
    ADD_METHOD_TO(DeptController::createDept,
                  "/dept",
                  drogon::Post,
                  drogon::Options);
    ADD_METHOD_VIA_REGEX(DeptController::updateDept,
                         "/dept/([1-9]\\d*)",
                         drogon::Patch,
                         drogon::Options);
    ADD_METHOD_VIA_REGEX(DeptController::deleteDept,
                         "/dept/([1-9]\\d*)",
                         drogon::Delete,
                         drogon::Options);
    ADD_METHOD_TO(DeptController::sortDept,
                  "/dept/sort",
                  drogon::Post,
                  drogon::Options);
    METHOD_LIST_END

    /**
     * @brief 获取部门树
     */
    drogon::Task<drogon::HttpResponsePtr> getDeptTree(
        const drogon::HttpRequestPtr req) const;

    /**
     * @brief 新增部门
     */
    drogon::Task<drogon::HttpResponsePtr> createDept(
        const drogon::HttpRequestPtr req,
        const DeptCreateRequest request) const;

    /**
     * @brief 更新部门
     */
    drogon::Task<drogon::HttpResponsePtr> updateDept(
        const drogon::HttpRequestPtr req,
        const std::int32_t deptId,
        const DeptUpdateRequest request) const;

    /**
     * @brief 删除部门
     */
    drogon::Task<drogon::HttpResponsePtr> deleteDept(
        const drogon::HttpRequestPtr req,
        const std::int32_t deptId) const;

    /**
     * @brief 排序部门
     */
    drogon::Task<drogon::HttpResponsePtr> sortDept(
        const drogon::HttpRequestPtr req,
        const DeptSortRequest request) const;

  private:
    DeptServicePtr deptService_{
        drogon::DrClassMap::getSingleInstance<DeptService>()};
};
