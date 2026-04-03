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
