import type { QuotaType, RelationType } from './enums'

export interface RoleDept {
  role_id: number
  dept_id: number
}

/**
 * 角色
 */
export interface Role {
  role_id: number
  name: string
  code: string
  description: string
  quota_type: QuotaType
  user_quota?: number
  relation_type: RelationType
  depts?: RoleDept[]
}

/**
 * 角色查询参数
 */
export interface RoleQueryParams {
  name: string
  dept_id?: number
}

/**
 * 新增/更新角色参数
 */
export interface RoleFormData {
  role_id?: number
  name?: string
  code?: string
  description?: string
  quota_type?: QuotaType
  user_quota?: number | null
  relation_type?: RelationType
  dept_ids?: number[] | null
}
