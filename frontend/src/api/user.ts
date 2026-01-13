import type {
  UserInfoUpdateRequest,
  UserPasswordUpdateRequest
} from '@/types/user'
import request from '@/utils/request'

/**
 * 更新用户信息接口
 */
export const updateUserInfo = async (
  userInfoUpdateRequest: UserInfoUpdateRequest
): Promise<undefined> => {
  await request.put(`/user/${userInfoUpdateRequest.user_id}`, {
    ...userInfoUpdateRequest,
    user_id: undefined
  })
}

/**
 * 更新用户密码接口
 */
export const updatePassword = async (
  userPasswordUpdateRequest: UserPasswordUpdateRequest
): Promise<undefined> => {
  await request.put(`/user/${userPasswordUpdateRequest.user_id}/password`, {
    ...userPasswordUpdateRequest,
    user_id: undefined
  })
}
