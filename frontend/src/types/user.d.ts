/**
 * 更新用户信息请求体
 *
 * undefined 表示不更新
 * null 表示更新为空
 */
export interface UserInfoUpdateRequest {
  nickname?: string
  sex?: 'male' | 'female' | 'secrecy'
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
