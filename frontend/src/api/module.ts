import request, { validateResponse } from '@/utils/request'
import type { Module } from '@/types/module'
import { isModuleTree } from '@/types/guard'

/**
 * 获取模块树
 */
export const getModuleTree = () => {
  return validateResponse<Module[]>(request.get('/module/tree'), isModuleTree)
}

/**
 * 删除指定模块
 */
export const deleteModule = (module_id: number) => {
  return request.delete(`/module/${module_id}`)
}
