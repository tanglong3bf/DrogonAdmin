/**
 * 响应体
 */
export interface ResponseBody<T> {
  data?: T
  message?: string | string[]
  warning?: string | string[]
  error?: string | string[]
  code?: int
}

/**
 * 标签页里的每一项
 */
export interface PageItem {
  name: string
  path: string
}

/**
 * 分页查询参数
 */
export interface Pagination {
  page: number
  page_size: number
}

/**
 * 分页查询结果
 */
export interface PaginatedResponse<T> {
  page: number
  page_size: number
  total: number
  list: T[]
}
