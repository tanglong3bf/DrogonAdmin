#pragma once

#include <vector>
#include "common/framework/domain/AuditableEntity.h"
#include "common/framework/domain/ChangeableEntity.h"
#include "common/util/ParamGetter.hpp"
#include "domain/models/SysMenu.h"

enum class MenuType : int16_t
{
    Directory,  ///< 菜单
    Page,       ///< 页面
    OutLink     ///< 外链
};

class Menu : public AuditableEntity, public ChangeableEntity
{
    using SysMenu = drogon_model::drogon_admin_db::SysMenu;

  public:
    /**
     * @brief 准备必备参数的构造
     */
    Menu(std::string_view name, std::int32_t sortNum, MenuType type);

    /**
     * @brief 准备必备参数以及创建者id的构造
     */
    Menu(std::string_view name,
         const std::int32_t sortNum,
         const MenuType type,
         const std::int32_t createdBy);

    /// @group 和model类互转
    /// @{
    explicit Menu(const SysMenu &model);
    explicit operator SysMenu() const;
    /// @}

    GETTER(menuId)
    GETTER(version)
    GETTER(actionId)
    GETTER(parentId)
    GETTER(name)
    GETTER(sortNum)
    GETTER(path)
    GETTER(icon)
    GETTER(type)
    GETTER(component)
    GETTER(children)

    void setParentId(const std::optional<std::int32_t> parentId);
    void setSortNum(std::int32_t sortNum);

    /**
     * @brief 更新排序号
     */
    void updateSortNum(std::int32_t sortNum, std::int32_t updatedBy)
    {
        sortNum_ = sortNum;
        markUpdatedBy(updatedBy);
        markUpdated();
    }

    /**
     * @brief 设置可选字段
     *
     * @param icon       图标
     * @param path       访问路径（Directory类型留空）
     * @param component  组件路径（仅Page类型有值）
     */
    void constructOptionalFields(const std::optional<std::string> &icon,
                                 const std::optional<std::string> &path,
                                 const std::optional<std::string> &component);

    /**
     * @brief 更新菜单基础信息
     */
    void updateBasicInfo(
        std::optional<std::string> name,
        const drogon_admin::util::NullableValue<std::int32_t> &parentId,
        std::int32_t sortNum,
        const drogon_admin::util::NullableValue<std::string> &icon,
        const drogon_admin::util::NullableValue<std::int32_t> &actionId,
        std::int32_t updatedBy);

    /**
     * @brief 更新菜单类型及类型相关的访问路径、组件
     *
     * 基于更新后的完整状态校验类型与字段的组合关系：
     * - 类型未传时沿用当前类型；path、component 未传时沿用当前值，null 表示清空
     * - Directory：清空访问路径与组件
     * - Page：必须设置访问路径与组件
     * - OutLink：必须设置访问路径，并清空组件
     */
    void updateTypeAndAccess(
        std::optional<MenuType> type,
        const drogon_admin::util::NullableValue<std::string> &path,
        const drogon_admin::util::NullableValue<std::string> &component,
        std::int32_t updatedBy);

    [[nodiscard]] bool isDirectory() const
    {
        return type_ == MenuType::Directory;
    }

    /**
     * @brief 软删除
     */
    void remove(const std::int32_t deletedBy);

  private:
    // 新增时没有id
    std::optional<std::int32_t> menuId_;    ///< 菜单id
    std::optional<int32_t> actionId_;       ///< 访问菜单需要的功能id
    std::optional<std::int32_t> parentId_;  ///< 父菜单
    std::optional<std::string> path_;       ///< 访问路径，仅在Menu时为空
    std::optional<std::string> icon_;       ///< 图标、可选
    std::string name_;                      ///< 菜单名称
    std::int32_t sortNum_;                  ///< 排序
    std::optional<std::string> component_;  ///< 组件路径，仅在Page时有值
    MenuType type_;                         ///< 菜单类型
    std::int32_t version_;                  ///< 乐观锁版本号
    std::vector<Menu> children_;            ///< 子菜单
};
