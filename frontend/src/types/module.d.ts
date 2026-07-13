/**
 * 功能项
 */
export interface Action {
  action_id: number
  name: string
  has_data_permission: boolean
  sort_num: number
}
/**
 * 模块
 */
export interface Module {
  module_id: number // 模块ID
  name: string // 模块名称
  description?: string // 模块描述
  sort_num: number // 模块排序
  parent_id?: number // 父级ID
  children?: Module[] // 子模块
  child_count?: number // 子模块数量
  actions?: Action[]
}

/**
 * 新增/更新模块参数
 */
export interface ModuleFormData {
  module_id?: number // 新增时为空，更新时需指定ID
  name: string // 模块名称（可更新）
  description?: string | null // 模块描述
  parent_id?: number // 新增时可设置父模块，不可更新
}

/**
 * 排序模块参数
 */
export interface ModuleSortItem {
  module_id: number // 模块ID
  name: string // 模块名称
  sort_num: number // 排序（数值小的靠前）
}
