import request, { validateResponse } from '@/utils/request'
import type { Role, RoleFormData, RoleQueryParams } from '@/types/role'
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
