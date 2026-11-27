import request, { validateResponse } from '@/utils/request'
import type {
  Menu,
  MenuSortFormDataItem,
  NewMenuData,
  UpdateMenuData
} from '@/types/menu'
import { isMenuTree } from '@/types/guard'

/**
 * 获取菜单树
 */
export const getMenuTree = () => {
  return validateResponse<Menu[]>(request.get('/menu/tree'), isMenuTree)
}

/**
 * 新增一个菜单项
 */
export const newMenu = (data: NewMenuData) => {
  return request.post('/menu', data)
}

/**
 * 更新指定菜单项
 */
export const updateMenu = (menu_id: number, data: UpdateMenuData) => {
  return request.patch(`/menu/${menu_id}`, data)
}

/**
 * 删除指定菜单项
 */
export const deleteMenu = (menu_id: number, version: number) => {
  return request.delete(`/menu/${menu_id}`, { params: { version } })
}

/**
 * 排序菜单
 */
export const sortMenu = (
  parent_id: number | undefined,
  menus: MenuSortFormDataItem[]
) => {
  return request.post('/menu/sort', { parent_id, menus })
}
