/**
 * 更新用户信息请求体
 *
 * undefined 表示不更新
 * null 表示更新为空
 */
export interface UserInfoUpdateRequest {
  user_id: number
  nickname?: string
  sex?: 'male' | 'female' | 'unknown'
  phone_number?: string | null
  email?: string | null
}

/**
 * 更新用户密码请求体
 */
export interface UserPasswordUpdateRequest {
  user_id: number
  oldPassword: string
  newPassword: string
}
