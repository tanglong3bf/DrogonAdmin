import { createRouter, createWebHistory, type RouteRecordRaw } from 'vue-router'
import { useCommonStore } from '@/stores/common'
import { useAuthStore } from '@/stores/auth'
import { SidebarMenu } from '@/types/menu'
import { PageItem } from '@/types/common'

const routes: RouteRecordRaw[] = [
  {
    path: '/',
    component: () => import('@/layouts/AdminLayout.vue'),
    redirect: '/home',
    name: 'root',
    children: [
      {
        path: '/home',
        component: () => import('@/views/Home.vue'),
        name: '首页'
      }
    ]
  },
  {
    path: '/login',
    component: () => import('@/views/Login.vue'),
    name: '登录页'
  }
]

const router = createRouter({
  history: createWebHistory(),
  routes
})

/**
 * 非法页面和登录检查
 */
router.beforeEach(to => {
  const authStore = useAuthStore()
  // 访问了不存在的页面
  if (to.matched.length === 0) {
    // 跳首页
    return '/home'
  }
  // 没登录
  if (to.path !== '/login' && authStore.token === undefined) {
    // 跳登录页
    return '/login'
  }
  return true
})

/**
 * 计算并设置当前激活的路径
 * 同时修改页面标题
 */
router.beforeEach(to => {
  const commonStore = useCommonStore()
  const menuList = commonStore.menuList

  /**
   * 根据欲跳转的路径计算activePath
   * @param items
   * @param toPath
   * @returns
   */
  const calActivePath = (items: SidebarMenu[], toPath: string): PageItem[] => {
    if (toPath === '/login') {
      return [{ name: '登录页', path: '/login' }]
    }
    // 获取节点的路径，无路径，递归查询第一个子节点
    const getEffectivePath = (item: SidebarMenu): string | undefined => {
      if (item.path) return item.path
      if (item.children && item.children.length > 0) {
        return getEffectivePath(item.children[0])
      }
      return undefined
    }
    for (const item of items) {
      if (item.path && item.path === toPath) {
        return [{ name: item.name, path: item.path }]
      }
      if (item.children && item.children.length > 0) {
        const childPathResult = calActivePath(item.children, toPath)
        if (childPathResult.length > 0) {
          const parentPageItem: PageItem = {
            name: item.name,
            path: getEffectivePath(item) || ''
          }
          return [parentPageItem].concat(childPathResult)
        }
      }
    }
    return []
  }
  const activePath = calActivePath(menuList, to.path)

  // 设置当前激活的路径
  commonStore.setActivePath(activePath)
  // 修改页面标题
  document.title = `${activePath[activePath.length - 1].name} - Drogon Admin`
  return true
})

export default router
