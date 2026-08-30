/**
 * 功能项
 */
export interface Action {
  action_id: number
  name: string
  code: string
  module_id: number
  has_data_permission: boolean
  sort_num: number
}

/**
 * 功能优先级
 */
export interface ActionPriority {
  high_id: number
  low_id: number
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
  version: number // 乐观锁版本号
  children?: Module[] // 子模块
  child_count?: number // 子模块数量
  actions?: Action[] // 功能
  priorities?: ActionPriority[] // 功能优先级
}

/**
 * 新增/更新模块参数
 */
export interface ModuleFormData {
  module_id?: number // 新增时为空，更新时需指定ID
  name: string // 模块名称（可更新）
  description?: string | null // 模块描述
  parent_id?: number // 新增时可设置父模块，不可更新
  version?: number // 乐观锁版本号
}

/**
 * 排序模块列表项
 */
export interface ModuleSortItem {
  module_id: number // 模块ID
  name: string // 模块名称
  version: number // 乐观锁版本号
}

/**
 * 排序模块参数
 */
export interface ModuleSortFormDataItem {
  module_id: number // 模块ID
  version: number // 乐观锁版本号
}
