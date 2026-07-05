#pragma once

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
    ADD_METHOD_VIA_REGEX(ModuleController::deleteModule,
                         "/module/([1-9]\\d*)",
                         drogon::Delete,
                         drogon::Options);
    METHOD_LIST_END

    /**
     * @brief 获取模块树
     */
    drogon::Task<drogon::HttpResponsePtr> getModuleTree(
        const drogon::HttpRequestPtr req) const;

    /**
     * @brief 删除模块
     */
    drogon::Task<drogon::HttpResponsePtr> deleteModule(
        const drogon::HttpRequestPtr req,
        const std::int32_t moduleId) const;

  private:
    ModuleServicePtr moduleService_{
        drogon::DrClassMap::getSingleInstance<ModuleService>()};
};
