import request, { validateResponse } from '@/utils/request'
import type { Department } from '@/types/department'
import { isDeptTree } from '@/types/guard'

/**
 * 获取部门树
 */
export const getDeptTree = () => {
  return validateResponse<Department[]>(request.get('/dept/tree'), isDeptTree)
}

/**
 * 新增一个部门
 */
export const newDept = (name: string, parent_id?: number) => {
  return request.post('/dept', { name, parent_id })
}

/**
 * 更新指定部门名称
 */
export const updateDept = (dept_id: number, name: string) => {
  return request.put(`/dept/${dept_id}`, { name })
}

/**
 * 删除指定部门
 */
export const deleteDept = (dept_id: number) => {
  return request.delete(`/dept/${dept_id}`)
}

/**
 * 排序部门
 */
export const sortDept = (parent_id: number | undefined, dept_ids: number[]) => {
  return request.put('/dept/sort', { parent_id, dept_ids })
}
