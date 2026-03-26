import request, { validateResponse } from '@/utils/request'
import type { Role, RoleQueryParams } from '@/types/role'
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
