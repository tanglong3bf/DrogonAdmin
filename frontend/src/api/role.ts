import request, { validateResponse } from '@/utils/request'
import type {
  Role,
  RoleFormData,
  RoleOption,
  RoleQueryParams
} from '@/types/role'
import { PaginatedResponse } from '@/types/common'
import { isPaginatedResponse, isRole } from '@/types/guard'

/**
 * 获取角色列表
 */
export const getRoleList = (queryParams: RoleQueryParams) => {
  return validateResponse<PaginatedResponse<Role>>(
    request.get('/role', { params: queryParams }),
    isPaginatedResponse(isRole)
  )
}

/**
 * 新增一个角色
 */
export const newRole = (data: RoleFormData) => {
  return request.post('/role', {
    code: data.code,
    name: data.name,
    description: data.description?.trim() || undefined,
    quota_type: data.quota_type,
    user_quota: data.quota_type !== 0 ? data.user_quota : undefined,
    relation_type: data.relation_type,
    dept_ids: data.relation_type !== 0 ? data.dept_ids : undefined
  })
}

/**
 * 更新角色
 */
export const updateRole = (role_id: number, role: RoleFormData) => {
  return request.put(`/role/${role_id}`, { ...role, role_id: undefined })
}

/**
 * 删除角色
 */
export const deleteRole = (role_id: number) => {
  return request.delete(`/role/${role_id}`)
}

/**
 * 获取指定部门可以使用的角色列表
 */
export const getAssignableRoles = (dept_id?: number): Promise<RoleOption[]> => {
  return request.get('/role/assignable', { params: { dept_id } })
}
