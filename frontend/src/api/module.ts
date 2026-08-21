import request, { validateResponse } from '@/utils/request'
import type {
  Action,
  ActionPriority,
  Module,
  ModuleSortFormDataItem
} from '@/types/module'
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
  version: number,
  name?: string,
  description?: string | null
) => {
  return request.patch(`/module/${module_id}`, { name, description, version })
}

/**
 * 删除指定模块
 */
export const deleteModule = (module_id: number, version: number) => {
  return request.delete(`/module/${module_id}`, { params: { version } })
}

/**
 * 排序模块
 */
export const sortModule = (
  parent_id: number | undefined,
  modules: ModuleSortFormDataItem[]
) => {
  return request.post('/module/sort', { parent_id, modules })
}

/**
 * 为模块分配功能
 */
export const assignAction = (
  module_id: number,
  version: number,
  actions: Action[],
  priorities: ActionPriority[]
) => {
  return request.post(`/module/${module_id}/actions`, {
    version,
    actions,
    priorities
  })
}
