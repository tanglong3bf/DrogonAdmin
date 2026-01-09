export interface LoginRequest {
  username: string
  password: string
}

export interface LoginResponse {
  token: string
  menu_list: MenuResponse[]
}

/**
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
