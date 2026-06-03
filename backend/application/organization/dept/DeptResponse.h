#pragma once

#include "domain/organization/dept/Dept.h"
#include <jsoncpp/json/value.h>
#include <string>
#include <vector>
#include <cstdint>

/**
 * @brief 部门响应数据封装
 */
class DeptResponse
{
  public:
    /**
     * @brief 实体类转当前类
     */
    DeptResponse(const Dept &dept);

    /**
     * @brief 转json
     */
    Json::Value toJson() const;

    GETTER(deptId)
    GETTER(parentId)
    GETTER(children)

    /**
     * @brief 新增一个子节点
     *
     * 内部为子节点设置parent
     */
    void addChild(DeptResponse &child);

  private:
    bool isChildIdExist(const std::int32_t id) noexcept;

  private:
    const std::int32_t deptId_;                   ///< 部门id
    const std::string name_;                      ///< 部门名称
    const std::int32_t sortNum_;                  ///< 排序
    const std::optional<std::int32_t> parentId_;  ///< 父部门id
    DeptResponse *parent_;                        ///< 父部门
    std::vector<DeptResponse> children_;          ///< 子部门列表
};
