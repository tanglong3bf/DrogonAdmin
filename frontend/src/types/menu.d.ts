import type { MenuType } from './enums'

/**
 * 菜单
 * 有children时，path和component必须为空
 * 无children时，path和component必须有值
 */
export interface Menu {
  menu_id: number
  type: MenuType
  path?: string
  icon?: string
  index: string
  name: string
  component?: string
  parent_id?: number // 父菜单ID，顶级菜单为空
  children?: Menu[]
  child_count?: number
  sort_num: number // 排序（数值小的靠前）
  version: number // 乐观锁版本号
  disabled?: boolean // Cascader 禁用状态
}

/**
 * 新增/更新菜单表单
 */
export interface MenuFormData {
  menu_id?: number // 新增时为空，更新时需指定ID
  type: MenuType // 菜单类型（新增时可选，更新时可修改）
  name: string // 菜单名称（可更新）
  path?: string // 访问路径（页面/外链必须有值）
  icon?: string // 图标（可更新）
  component?: string // 组件（页面必须有值）
  parent_id?: number // 新增时可设置父菜单，更新时也可修改
  version?: number // 乐观锁版本号（更新时必须）
  /** 原始菜单ID，用于排除自己和子菜单作为父菜单选项 */
  originalMenuId?: number
  /** 是否包含子菜单，用于限制目录类型不可更改 */
  hasChildren?: boolean
  /** 子菜单数量（用于更新时限制类型切换） */
  child_count?: number
}

/**
 * 新增菜单参数
 */
export interface NewMenuData {
  name: string // 菜单名称
  type: MenuType // 菜单类型
  path?: string // 访问路径（页面/外链必填）
  icon?: string // 图标
  component?: string // 组件（页面必填）
  parent_id?: number // 父菜单ID
}

/**
 * 更新菜单参数
 */
export interface UpdateMenuData {
  version: number // 乐观锁版本号
  name: string // 菜单名称
  type?: MenuType // 菜单类型（可选，不传则不变）
  icon?: string | null // 图标，null表示清空
  path?: string | null // 访问路径，null表示清空
  component?: string | null // 组件，null表示清空
  parent_id?: number | null // 父菜单ID，null表示设为顶级
}

/**
 * 排序菜单列表项
 */
export interface MenuSortItem {
  menu_id: number // 菜单ID
  name: string // 菜单名称
  version: number // 乐观锁版本号
}

/**
 * 排序菜单参数
 */
export interface MenuSortFormDataItem {
  menu_id: number // 菜单ID
  version: number // 乐观锁版本号
}
