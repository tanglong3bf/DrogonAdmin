import axios, { type AxiosResponse, type AxiosError } from 'axios'
import { useAuthStore } from '@/stores/auth'
import { ElMessage } from 'element-plus'
import { ResponseBody } from '@/types/common'

const authStore = useAuthStore()

const instance = axios.create({
  baseURL: 'http://localhost:8000',
  timeout: 5000
})

/**
 * 批量提示信息
 */
const ElMessageList = (
  message: string | string[],
  type: 'info' | 'warning' | 'error'
) => {
  if (typeof message === 'string') {
    ElMessage({ message, type })
  } else {
    message.forEach(message =>
      ElMessage({
        message,
        type
      })
    )
  }
}

/**
 * 每次请求时添加 token
 */
instance.interceptors.request.use(
  config => {
    const token = authStore.token
    if (token !== undefined) {
      config.headers['Authorization'] = `Bearer ${token}`
    }
    return config
  },
  error => {
    return Promise.reject(error)
  }
)

/**
 * 响应拦截器
 */
instance.interceptors.response.use(
  (response: AxiosResponse<ResponseBody<any>>) => {
    // 204 响应体为空
    if (response.status === 204) {
      return undefined
    }

    // 正常响应
    if (response.status >= 200 && response.status < 300) {
      const responseData = response.data || {}
      const { data, message, warning, error, code } = responseData

      // 2xx 不应有error
      if (error !== undefined) {
        ElMessageList(error, 'error')
        return Promise.reject(
          new Error(
            `2xx响应异常携带错误信息：${typeof error === 'string' ? error : error.join(', ')}`
          )
        )
      }

      // 2xx code可选，有的话必须非负
      if (code !== undefined && code < 0) {
        ElMessage({
          message: `2xx响应异常：code(${code}) 不应小于零`,
          type: 'error'
        })
        return Promise.reject(new Error(`2xx响应code非法：${code}`))
      }

      // 201 默认提示创建成功
      if (response.status === 201) {
        if (message === undefined) {
          ElMessage({ message: '创建成功', type: 'info' })
        } else {
          ElMessageList(message, 'info')
        }
      } else {
        // 其他2xx状态码
        if (message !== undefined) {
          ElMessageList(message, 'info')
        } else if (warning !== undefined) {
          ElMessageList(warning, 'warning')
        }
      }

      // 2xx正常返回数据
      return data
    }
  },
  (error: AxiosError<ResponseBody<unknown>>) => {
    const response = error.response
    // 4xx/5xx
    if (response && response.status >= 400 && response.status < 600) {
      const responseData = response.data || {}
      const { error: resError, code, message, warning } = responseData

      // 4xx/5xx code必须有且为负值
      if (code === undefined || code >= 0) {
        ElMessage.error('未知错误')
        return Promise.reject(new Error(`4xx/5xx响应code非法：${code}`))
      }

      // 4xx/5xx 不可以有message、warn
      if (message !== undefined || warning !== undefined) {
        ElMessage.error('未知错误')
        return Promise.reject(
          new Error(
            `4xx/5xx响应异常携带非法字段：message=${message}, warning=${warning}（不符合规范要求）`
          )
        )
      }

      // 4xx/5xx error可选
      if (resError !== undefined) {
        ElMessageList(resError, 'error')
      } else {
        ElMessage({
          message: `请求失败(${response.status})：${code}`,
          type: 'error'
        })
      }

      // 401表示token无效或已过期，清除token
      if (response.status === 401) {
        authStore.setToken(undefined)
      }
    } else {
      // 无响应
      ElMessage.error('网络错误或服务器未响应')
    }

    return Promise.reject(error)
  }
)

/**
 * 响应体格式检查
 */
export async function validateResponse<T>(
  promise: Promise<T>,
  validator: (data: unknown) => data is T
): Promise<T> {
  const response = await promise
  if (!validator(response)) {
    ElMessage.error('未知错误')
    throw new Error('响应体格式错误')
  }
  return response
}

export default instance
