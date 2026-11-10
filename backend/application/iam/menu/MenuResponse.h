#pragma once

#include "domain/iam/menu/Menu.h"
#include <drogon/HttpResponse.h>
#include <jsoncpp/json/value.h>
#include <string>
#include <vector>
#include <cstdint>

/**
 * @brief 菜单响应数据封装
 */
class MenuResponse
{
  public:
    /**
     * @brief 实体类转当前类
     */
    explicit MenuResponse(const Menu &menu);

    /**
     * @brief 转json
     */
    Json::Value toJson() const;

    GETTER(menuId)
    GETTER(sortNum)
    GETTER(version)
    GETTER(parentId)
    GETTER(children)

    /**
     * @brief 新增一个子节点
     *
     * 内部为子节点设置parent
     */
    void addChild(MenuResponse &child);

  private:
    bool isChildIdExist(const std::int32_t id) noexcept;

  private:
    const std::int32_t menuId_;                   ///< 菜单id
    const std::int32_t sortNum_;                  ///< 排序
    const std::int32_t version_;                  ///< 乐观锁版本号
    const std::optional<std::int32_t> parentId_;  ///< 父菜单id
    const std::string name_;                      ///< 菜单名称
    const std::optional<std::string> path_;       ///< 访问路径
    const std::optional<std::string> icon_;       ///< 图标
    const short type_;                            ///< 菜单类型
    const std::optional<std::string> component_;  ///< 组件路径
    MenuResponse *parent_;                        ///< 父菜单
    std::vector<MenuResponse> children_;          ///< 子菜单列表
};

/**
 * @brief 将菜单响应列表转为Json数组
 */
Json::Value toJsonArray(const std::vector<MenuResponse> &data);

namespace drogon
{
template <>
HttpResponsePtr toResponse(const std::vector<MenuResponse> &response);
}  // namespace drogon
