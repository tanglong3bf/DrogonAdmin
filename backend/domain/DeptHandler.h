#pragma once

#include <memory>
#include <drogon/utils/coroutine.h>
#include "common/framework/DrAdminObject.hpp"
#include "domain/Dept.h"
#include "domain/DeptVerifier.h"

/**
 * @brief 部门处理器
 */
class DeptHandler : public DrAdminObject<DeptHandler>
{
  public:
    /**
     * @brief 更新部门
     */
    drogon::Task<> updateDept(Dept &dept,
                              const std::string &newName,
                              const std::uint32_t updatedBy) const;

  private:
    void validateNameNotSame(const std::string &oldName,
                             const std::string &newName) const;

  private:
    DeptVerifierPtr deptVerifier_{
        drogon::DrClassMap::getSingleInstance<DeptVerifier>()};
};

using DeptHandlerPtr = std::shared_ptr<DeptHandler>;
