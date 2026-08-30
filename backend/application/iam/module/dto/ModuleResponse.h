#pragma once

#include "ActionResponse.h"
#include "ActionPriorityResponse.h"
#include "domain/iam/module/Module.h"
#include <optional>
#include <string>
#include <vector>
#include <cstdint>

/**
 * @brief 模块响应
 */
class ModuleResponse
{
  public:
    /**
     * @brief 实体类转当前类
     */
    ModuleResponse(const Module &module);

    /**
     * @brief 转json
     */
    Json::Value toJson() const;

    // getters
    GETTER(moduleId)
    GETTER_STR_VIEW(name)
    GETTER(description)
    GETTER(sortNum)
    GETTER(parentId)
    GETTER(version)
    GETTER(children)
    GETTER(actions)
    GETTER(actionPriorities)

    void appendAction(ActionResponse &action);

    void appendPriority(ActionPriorityResponse &priority);

    /**
     * @brief 新增一个子节点
     *
     * 内部为子节点设置parent
     */
    void addChild(ModuleResponse &child);

  private:
    bool isChildIdExist(const std::int32_t id) noexcept;

  private:
    const std::int32_t moduleId_;                           ///< 模块id
    const std::string name_;                                ///< 模块名称
    const std::optional<std::string> description_;          ///< 模块描述
    const std::int32_t sortNum_;                            ///< 模块排序
    const std::optional<std::int32_t> parentId_;            ///< 父模块id
    const std::int32_t version_;                            ///< 乐观锁版本号
    ModuleResponse *parent_;                                ///< 父模块
    std::vector<ModuleResponse> children_;                  ///< 子模块列表
    std::vector<ActionResponse> actions_;                   ///< 功能列表
    std::vector<ActionPriorityResponse> actionPriorities_;  ///< 功能优先级
};
