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
