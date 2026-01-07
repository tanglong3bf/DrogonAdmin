#pragma once

enum class ChangingStatus
{
    UNCHANGED,  ///< 未修改
    NEW,        ///< 新增
    UPDATED,    ///< 更新
    DELETED     ///< 删除
};

/**
 * @brief 实体类修改状态
 *
 * @note 大多数实体类不需要，典型应用场景：聚合修改
 */
class ChangeableEntity
{
  public:
    ChangeableEntity();

    ChangingStatus getChangingStatus() const;
    void toUpdate();
    void toDelete();
    void toUnChang();

  private:
    ChangingStatus changingStatus_;
};
