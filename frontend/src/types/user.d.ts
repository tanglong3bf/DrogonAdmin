import { Sex } from 'enums'
import { Status } from 'enums'

export interface UserRole {
  user_id: number
  role_id: number
}

/**
 * 用户
 */
export interface User {
  user_id: number
  avatar: string
  username: string
  nickname: string
  sex: Sex
  dept_id: number
  phone_number?: string
  email?: string
  status: Status
  user_roles: UserRole[]
}

/**
 * 新增/更新用户参数
 */
export interface UserFormData {
  user_id?: number
  username?: string
  nickname?: string
  sex?: Sex
  dept_id?: number
  phone_number?: string | null
  email?: string | null
  status?: Status
  role_ids?: number[]
}

/**
 * 更新用户信息请求体
 *
 * undefined 表示不更新
 * null 表示更新为空
 */
export interface UserInfoUpdateRequest {
  nickname?: string
  sex?: Sex
  phone_number?: string | null
  email?: string | null
}

/**
 * 更新用户密码请求体
 */
export interface UserPasswordUpdateRequest {
  oldPassword: string
  newPassword: string
}

export interface UploadAvatarResponse {
  file_path: string
}

export interface UserQueryParams {
  username?: string
  nickname?: string
  sex?: Sex
  dept_id?: number
  phone_number?: string
  email?: string
  status?: number
}
