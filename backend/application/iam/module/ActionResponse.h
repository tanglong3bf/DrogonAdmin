#pragma once

#include "domain/iam/module/Action.h"
#include <string>

/**
 * @brief 模块响应
 */
class ActionResponse
{
  public:
    /**
     * @brief 实体类转当前类
     */
    ActionResponse(const Action &action);

    /**
     * @brief 转json
     */
    Json::Value toJson() const;

    // getters
    GETTER(actionId)
    GETTER_STR_VIEW(name)
    GETTER_STR_VIEW(code)
    GETTER(description)
    GETTER(sortNum)
    GETTER(moduleId)

  private:
    std::int32_t actionId_;
    std::string name_;
    std::string code_;
    std::optional<std::string> description_;
    std::int32_t sortNum_;
    std::int32_t moduleId_;
};
