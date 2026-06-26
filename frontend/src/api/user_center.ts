import { isUploadAvatarResponse } from '@/types/guard'
import type {
  UploadAvatarResponse,
  UserInfoUpdateRequest,
  UserPasswordUpdateRequest
} from '@/types/user'
import request, { validateResponse } from '@/utils/request'
/**
 * 用户修改自己的基础信息
 */
export const updateUserBasicInfo = (
  userInfoUpdateRequest: UserInfoUpdateRequest
): Promise<undefined> => {
  return request.patch('/user_center/basic_info', {
    ...userInfoUpdateRequest
  })
}

/**
 * 更新用户密码接口
 */
export const changePassword = (
  userPasswordUpdateRequest: UserPasswordUpdateRequest
): Promise<undefined> => {
  return request.patch('/user_center/change_password', {
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
    request.post('/user_center/upload_avatar', formData, {
      headers: { 'Content-Type': 'multipart/form-data' }
    }),
    isUploadAvatarResponse
  )
}
