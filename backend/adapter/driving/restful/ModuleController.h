#pragma once

#include "application/iam/module/dto/ModuleCreateRequest.h"
#include "application/iam/module/dto/ModuleUpdateRequest.h"
#include "application/iam/module/dto/ModuleSortRequest.h"
#include "application/iam/module/dto/ActionUpdateRequest.h"
#include "application/iam/module/ModuleService.h"
#include <drogon/HttpController.h>

class ModuleController : public drogon::HttpController<ModuleController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(ModuleController::getModuleTree,
                  "/module/tree",
                  drogon::Get,
                  drogon::Options);
    ADD_METHOD_TO(ModuleController::createModule,
                  "/module",
                  drogon::Post,
                  drogon::Options);
    ADD_METHOD_VIA_REGEX(ModuleController::updateModule,
                         "/module/([1-9]\\d*)",
                         drogon::Patch,
                         drogon::Options);
    ADD_METHOD_VIA_REGEX(ModuleController::deleteModule,
                         "/module/([1-9]\\d*)",
                         drogon::Delete,
                         drogon::Options);
    ADD_METHOD_VIA_REGEX(ModuleController::sortModule,
                         "/module/sort",
                         drogon::Post,
                         drogon::Options);
    ADD_METHOD_VIA_REGEX(ModuleController::updateActions,
                         "/module/([1-9]\\d*)/actions",
                         drogon::Post,
                         drogon::Options);
    METHOD_LIST_END

    /**
     * @brief 获取模块树
     */
    drogon::Task<drogon::HttpResponsePtr> getModuleTree(
        const drogon::HttpRequestPtr req) const;

    /**
     * @brief 新增模块
     */
    drogon::Task<drogon::HttpResponsePtr> createModule(
        const drogon::HttpRequestPtr req,
        const ModuleCreateRequest request) const;

    /**
     * 更新模块
     */
    drogon::Task<drogon::HttpResponsePtr> updateModule(
        const drogon::HttpRequestPtr req,
        const std::int32_t moduleId,
        const ModuleUpdateRequest request) const;

    /**
     * @brief 删除模块
     */
    drogon::Task<drogon::HttpResponsePtr> deleteModule(
        const drogon::HttpRequestPtr req,
        const std::int32_t moduleId) const;

    /**
     * @brief 排序模块
     */
    drogon::Task<drogon::HttpResponsePtr> sortModule(
        const drogon::HttpRequestPtr req,
        const ModuleSortRequest request) const;

    /**
     * @brief 更新功能及优先级
     */
    drogon::Task<drogon::HttpResponsePtr> updateActions(
        const drogon::HttpRequestPtr req,
        const std::int32_t moduleId,
        const ActionUpdateRequest request) const;

  private:
    ModuleServicePtr moduleService_{
        drogon::DrClassMap::getSingleInstance<ModuleService>()};
};
