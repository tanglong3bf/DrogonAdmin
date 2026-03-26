export interface Role {
  role_id: number
  name: string
  code: string
  description: string
  user_quota?: number
  quota_type: 'Unlimited' | 'TotalLimit' | 'PerDeptLimit'
}

export interface RoleQueryParams {
  name?: string
  dept_id?: number
  page: number
  page_size: number
}
