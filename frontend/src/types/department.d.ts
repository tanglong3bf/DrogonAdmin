/**
 * 部门
 */
export interface Department {
  dept_id: number // 部门ID
  name: string // 部门名称
  parent_id?: number // 父部门ID
  sort_num: number // 排序（数值小的靠前）
  children?: Department[] // 子部门
  child_count?: number // 子部门数量
}

/**
 * 新增/更新部门参数
 */
export interface DeptFormData {
  name: string // 部门名称（可更新）
  parent_id?: number // 新增时可设置父部门，不可更新
  dept_id?: number // 更新时需指定ID
}

/**
 * 排序部门参数
 */
export interface DeptSortItem {
  dept_id: number // 部门ID
  name: string // 部门名称
  sort_num: number // 排序（数值小的靠前）
}
