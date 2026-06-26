import { PaginatedResponse } from '@/types/common'
import { isPaginatedResponse, isUser } from '@/types/guard'
import type { User, UserFormData, UserQueryParams } from '@/types/user'
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
 * 新增一个用户
 */
export const newUser = (data: UserFormData) => {
  return request.post('/user', data)
}

/**
 * 更新一个用户
 */
export const updateUser = (user_id: number, user: UserFormData) => {
  return request.patch(`/user/${user_id}`, { ...user, user_id: undefined })
}

/**
 * 删除用户
 */
export const deleteUser = (user_id: number) => {
  return request.delete(`/user/${user_id}`)
}
