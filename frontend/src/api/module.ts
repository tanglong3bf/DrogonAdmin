import request, { validateResponse } from '@/utils/request'
import type { Action, Module } from '@/types/module'
import { isModuleTree } from '@/types/guard'

/**
 * 获取模块树
 */
export const getModuleTree = () => {
  return validateResponse<Module[]>(request.get('/module/tree'), isModuleTree)
}

/**
 * 新增一个模块
 */
export const newModule = (
  name: string,
  description?: string,
  parent_id?: number
) => {
  return request.post('/module', { name, description, parent_id })
}

/**
 * 更新指定模块
 */
export const updateModule = (
  module_id: number,
  name?: string,
  description?: string | null
) => {
  return request.patch(`/module/${module_id}`, { name, description })
}

/**
 * 删除指定模块
 */
export const deleteModule = (module_id: number) => {
  return request.delete(`/module/${module_id}`)
}

/**
 * 排序模块
 */
export const sortModule = (
  parent_id: number | undefined,
  module_ids: number[]
) => {
  return request.post('/module/sort', { parent_id, module_ids })
}

/**
 * 为模块分配功能
 */
export const assignAction = (
  module_id: number,
  actions: Action[],
  actionsLink: { high: number; low: number }[]
) => {
  return request.post(`/module/${module_id}/actions`, {
    actions,
    actionsLink
  })
}
