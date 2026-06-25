/**
 * 限制类型
 */
export enum QuotaType {
  Unlimited = 0,
  TotalLimit = 1,
  PerDeptLimit = 2
}

/**
 * 角色部门关联类型
 */
export enum RelationType {
  All = 0,
  Whitelist = 1,
  Blacklist = 2
}

/**
 * 用户性别
 */
export enum Sex {
  Secrecy = 0,
  Male,
  Female
}

/**
 * 用户状态
 */
export enum Status {
  Enable = 0,
  Disable
}

/**
 * 对话框类型枚举
 */
export enum DialogType {
  ADD = 0, // 新增
  UPDATE = 1 // 更新
}

/**
 * 菜单类型
 */
export enum MenuType {
  Menu = 0, // 菜单
  Page = 1, // 页面
  OutLink = 2 // 外链
}
