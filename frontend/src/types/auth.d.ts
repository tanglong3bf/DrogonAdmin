/**
 * 登录请求体
 */
export interface LoginRequest {
  username: string
  password: string
}

/**
 * 登录响应
 */
export interface LoginResponse {
  token: string
  user_info: UserInfo
  menu_list: MenuResponse[]
}

/**
 * 用户信息
 */
export interface UserInfo {
  user_id: number
  username: string
  nickname: string
  avatar: string
  sex: 'male' | 'female' | 'secrecy'
  deparemnt_id: number
  roles_id: number[]
  phone_number: string
  email: string
  status: 'normal' | 'disabled'
}

/**
 * 登录时返回的目录树
 *
 * 当type为menu时，path和component必须为空，children可选
 * 当type为page时，path和component必须有值，children为空
 * 当type为out_link时，path必须有值，component和children为空
 * 注意path的格式
 */
export interface MenuResponse {
  menu_id: number
  icon?: string
  name: string
  type: 'menu' | 'page' | 'out_link'
  path?: string
  component?: string
  children?: MenuResponse[]
}
