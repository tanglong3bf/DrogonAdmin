import { Department } from './department'

/**
 * 限制类型
 */
enum QuotaType {
  Unlimited = 0,
  TotalLimit = 1,
  PerDeptLimit = 2
}

/**
 * 角色部门关联类型
 */
enum RelationType {
  All = 0,
  Whitelist = 1,
  Blacklist = 2
}

export interface RoleDept {
  id: number
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
  user_quota?: number
  relation_type?: RelationType
  dept_ids?: number[]
}
