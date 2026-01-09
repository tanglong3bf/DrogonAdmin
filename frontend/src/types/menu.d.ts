/**
 * 侧边栏菜单
 * 有children时，path和component必须为空
 * 无children时，path和component必须有值
 */
export interface SidebarMenu {
  path?: string
  icon?: string
  index: string
  name: string
  component?: string
  children?: SidebarMenu[]
}
