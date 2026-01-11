#pragma once

#include <drogon/HttpController.h>
#include <drogon/DrClassMap.h>
#include "application/DeptService.h"

class DeptController : public drogon::HttpController<DeptController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(DeptController::getDeptTree,
                  "/dept/tree",
                  drogon::Get,
                  drogon::Options);
    METHOD_LIST_END

    /**
     * @brief 获取部门树
     */
    drogon::Task<drogon::HttpResponsePtr> getDeptTree(
        const drogon::HttpRequestPtr req) const;

  private:
    DeptServicePtr deptService_{
        drogon::DrClassMap::getSingleInstance<DeptService>()};
};
