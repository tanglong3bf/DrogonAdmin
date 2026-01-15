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
 */
class ChangeableEntity
{
  public:
    ChangeableEntity();

    ChangingStatus getChangingStatus() const;
    void toNew();
    void toUpdate();
    void toDelete();

  private:
    ChangingStatus changingStatus_;
};
