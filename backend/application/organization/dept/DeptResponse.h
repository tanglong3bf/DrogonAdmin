#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <jsoncpp/json/value.h>
#include "domain/organization/dept/Dept.h"

/**
 * @brief 部门响应数据封装
 */
class DeptResponse
{
    std::int32_t deptId_;                   ///< 部门id
    std::string name_;                      ///< 部门名称
    std::int32_t sortNum_;                  ///< 排序
    std::optional<std::int32_t> parentId_;  ///< 父部门id
    DeptResponse *parent_;                  ///< 父部门
    std::vector<DeptResponse> children_;    ///< 子部门列表

  public:
    /**
     * @brief 实体类转当前类
     */
    DeptResponse(const Dept &dept);

    /**
     * @brief 转json
     */
    Json::Value toJson() const;

    // setters
    SETTER(parent, Parent)

    /**
     * @brief 新增一个子节点
     *
     * 内部为子节点设置parent_
     */
    void addChild(DeptResponse &child);

    // getters
    GETTER(deptId, DeptId)
    OPT_GETTER(parentId, ParentId)
    GETTER(children, Children)

  private:
    bool isChildIdExist(const std::int32_t id) noexcept;
};
