#pragma once

#include "domain/iam/module/ActionPriority.h"
#include "common/util/Utilities.hpp"
#include <json/value.h>

class ActionPriorityResponse
{
  public:
    /**
     * @brief 实体类转当前类
     */
    ActionPriorityResponse(const ActionPriority &actionPriority);

    /**
     * @brief 转json
     */
    Json::Value toJson() const;

    // getters
    GETTER(highId)
    GETTER(lowId)

  private:
    std::int32_t highId_;
    std::int32_t lowId_;
};
