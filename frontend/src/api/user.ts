import { PaginatedResponse } from '@/types/common'
import {
  isPaginatedResponse,
  isUploadAvatarResponse,
  isUser
} from '@/types/guard'
import type {
  UploadAvatarResponse,
  User,
  UserInfoUpdateRequest,
  UserPasswordUpdateRequest,
  UserQueryParams
} from '@/types/user'
import request, { validateResponse } from '@/utils/request'

/**
 * 获取用户列表
 */
export const getUserList = (queryParams: UserQueryParams) => {
  return validateResponse<PaginatedResponse<User>>(
    request.get('/user', { params: queryParams }),
    isPaginatedResponse(isUser)
  )
}

/**
 * 更新用户信息接口
 */
export const updateUserInfo = (
  userInfoUpdateRequest: UserInfoUpdateRequest
): Promise<undefined> => {
  return request.put('/user', {
    ...userInfoUpdateRequest
  })
}

/**
 * 更新用户密码接口
 */
export const updatePassword = (
  userPasswordUpdateRequest: UserPasswordUpdateRequest
): Promise<undefined> => {
  return request.put('/user/password', {
    ...userPasswordUpdateRequest
  })
}

/**
 * 上传用户头像
 */
export const uploadAvatar = (file: File) => {
  const formData = new FormData()
  formData.append('avatar', file)
  return validateResponse<UploadAvatarResponse>(
    request.post('/user/upload_avatar', formData, {
      headers: { 'Content-Type': 'multipart/form-data' }
    }),
    isUploadAvatarResponse
  )
}
