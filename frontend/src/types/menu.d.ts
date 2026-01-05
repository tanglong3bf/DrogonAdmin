/**
 * 侧边栏菜单
 */
export interface SidebarMenu {
  path: string
  icon?: string
  index: string
  name: string
  sub_menu?: SidebarMenu[]
}
