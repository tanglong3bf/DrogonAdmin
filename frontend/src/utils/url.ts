import { IMG_BASE_URL } from '@/config'

/**
 * 拼接图片完整url，自动处理首尾斜杠，避免 //
 * @param baseUrl 基础域名，如 http://localhost:8000
 * @param relativePath 后端返回相对路径：uploads/a.jpg 或者 /uploads/a.jpg
 */
export function joinImageUrl(relativePath: string): string {
  // 去掉baseUrl末尾的 /
  const base = IMG_BASE_URL.replace(/\/+$/, '')
  // 去掉相对路径开头的 /
  const path = relativePath.replace(/^\/+/, '')
  return `${base}/${path}`
}
