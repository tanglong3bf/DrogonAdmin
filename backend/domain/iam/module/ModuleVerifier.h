#pragma once

#include "domain/iam/module/ModuleRepository.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/utils/coroutine.h>

class ModuleVerifier : public DrAdminObject<ModuleVerifier>
{
  public:
    /**
     * @brief 校验模块没有子模块
     */
    drogon::Task<> verifyNoSubmodule(const int32_t moduleId) const;

    /**
     * @brief 校验模块存在
     */
    drogon::Task<> verifyModuleExists(const int32_t moduleId) const;

    /**
     * @brief 校验模块名称未被使用
     */
    drogon::Task<> verifyModuleNameNotDuplicated(
        const std::string &name,
        const std::optional<int32_t> parentId) const;

    /**
     * @brief 校验功能属于模块
     */
    drogon::Task<> verifyActionsBelongsToModule(
        std::int32_t moduleId,
        const std::vector<std::int32_t> &actionIds) const;

  private:
    ModuleRepositoryPtr moduleRepository_{
        drogon::DrClassMap::getSingleInstance<ModuleRepository>()};
};

using ModuleVerifierPtr = std::shared_ptr<ModuleVerifier>;
