#pragma once

#include <drogon/utils/coroutine.h>
#include <memory>
#include <vector>
#include "application/DeptCqrsRepo.h"
#include "common/framework/DrAdminObject.hpp"
#include "DeptResponse.h"

class DeptService : public DrAdminObject<DeptService>
{
  public:
    DeptService() = default;

  public:
    /**
     * @brief 获取部门树
     */
    drogon::Task<std::vector<DeptResponse>> getDeptTree() const;

  private:
    DeptCqrsRepoPtr deptCqrsRepo_{
        drogon::DrClassMap::getSingleInstance<DeptCqrsRepo>()};
};

using DeptServicePtr = std::shared_ptr<DeptService>;
