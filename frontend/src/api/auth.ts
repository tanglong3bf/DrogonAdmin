import request, { validateResponse } from '@/utils/request'
import type { LoginRequest, LoginResponse } from '@/types/auth'
import { isLoginResponse } from '@/types/guard'

/**
 * 登录接口
 */
export const login = (loginRequest: LoginRequest) => {
  return validateResponse<LoginResponse>(
    request.post('/login', loginRequest),
    isLoginResponse
  )
}
