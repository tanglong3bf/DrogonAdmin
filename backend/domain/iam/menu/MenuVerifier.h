#pragma once

#include "MenuRepository.h"
#include "common/framework/DrAdminObject.hpp"
#include <drogon/utils/coroutine.h>

class MenuVerifier : public DrAdminObject<MenuVerifier>
{
  public:
    /**
     * @brief 校验指定菜单是否存在（未删除）
     */
    drogon::Task<> verifyMenuIsDirectory(const std::int32_t menuId) const;

    /**
     * @brief 校验同级下菜单名称是否重复
     */
    drogon::Task<> verifyMenuNameNotDuplicated(
        const std::string &name,
        const std::optional<std::int32_t> &parentId) const;

    drogon::Task<> verifyActionNotUsed(
        const std::vector<std::int32_t> &actionIds) const;

    /**
     * @brief 校验目标父菜单不是当前菜单自身或其子孙节点
     */
    drogon::Task<> verifyParentNotSelfOrDescendant(
        std::int32_t menuId,
        std::int32_t parentId) const;

  private:
    MenuRepositoryPtr menuRepository_{
        drogon::DrClassMap::getSingleInstance<MenuRepository>()};
};

using MenuVerifierPtr = std::shared_ptr<MenuVerifier>;
