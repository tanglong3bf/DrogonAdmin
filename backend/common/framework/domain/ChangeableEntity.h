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
    ChangingStatus changingStatus() const noexcept;
    bool isNew() const noexcept;
    bool isUpdated() const noexcept;
    bool isDeleted() const noexcept;
    void markNew();
    void markUpdated();
    void markDeleted();

  private:
    ChangingStatus changingStatus_{ChangingStatus::UNCHANGED};
};
