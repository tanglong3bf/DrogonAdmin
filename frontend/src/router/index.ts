import { useCommonStore } from '@/stores/common'
import type { PageItem } from '@/types/common'
import { createRouter, createWebHistory, type RouteRecordRaw } from 'vue-router'

// TODO: 登录时设置，临时写的测试用
const routes: RouteRecordRaw[] = [
  {
    path: '/',
    component: () => import('@/layouts/AdminLayout.vue'),
    redirect: '/home',
    children: [
      {
        path: '/home',
        component: () => import('@/views/Home.vue'),
        name: '首页'
      },
      {
        path: '/sys',
        redirect: '/sys/dept_user',
        name: '系统管理',
        children: [
          {
            path: '/sys/dept_user',
            redirect: '/sys/dept_user/dept',
            name: '部门用户',
            children: [
              {
                path: '/sys/dept_user/dept',
                component: () => import('@/views/sys/dept_user/Dept.vue'),
                name: '部门管理'
              },
              {
                path: '/sys/dept_user/user',
                component: () => import('@/views/sys/dept_user/User.vue'),
                name: '用户管理'
              }
            ]
          },
          {
            path: '/sys/module_menu',
            redirect: '/sys/module_menu/menu',
            name: '模块菜单',
            children: [
              {
                path: '/sys/module_menu/menu',
                component: () => import('@/views/sys/module_menu/Menu.vue'),
                name: '模块管理'
              },
              {
                path: '/sys/module_menu/module',
                component: () => import('@/views/sys/module_menu/Module.vue'),
                name: '菜单管理'
              }
            ]
          },
          {
            path: '/sys/role_auth',
            redirect: '/sys/role_auth/role',
            name: '角色权限',
            children: [
              {
                path: '/sys/role_auth/role',
                component: () => import('@/views/sys/role_auth/Role.vue'),
                name: '角色管理'
              },
              {
                path: '/sys/role_auth/auth',
                component: () => import('@/views/sys/role_auth/Auth.vue'),
                name: '权限管理'
              }
            ]
          }
        ]
      }
    ]
  }
]

const router = createRouter({
  history: createWebHistory(),
  routes
})

/**
 * 计算并设置当前激活的路径
 * 同时修改页面标题
 */
router.beforeEach(to => {
  /**
   * 根据欲跳转的路径计算activePath
   * @param path
   * @returns
   */
  const calActivePath = (path: string): PageItem[] => {
    // 分割路径为各个部分
    const pathSegments = path.split('/').filter(segment => segment !== '')

    // 构建路径数组
    const result: PageItem[] = pathSegments.reduce(
      (acc: PageItem[], segment: string) => {
        const currentPath =
          acc.length > 0
            ? `${acc[acc.length - 1].path}/${segment}`
            : `/${segment}`
        return [...acc, { name: '', path: currentPath }]
      },
      []
    )

    // 查找每个路径对应的名称
    const findRouteName = (routes: RouteRecordRaw[], path: string): string =>
      routes.reduce((name: string, route: RouteRecordRaw): string => {
        if (name) return name
        if (route.path === path) return (route.name as string) || ''
        return route.children ? findRouteName(route.children, path) : ''
      }, '')

    // 更新每个路径的名称
    const updateRouteNames = (routes: RouteRecordRaw[], result: PageItem[]) =>
      result.map(item => ({ ...item, name: findRouteName(routes, item.path) }))

    return updateRouteNames(routes, result)
  }
  const activePath = calActivePath(to.path)

  // 设置当前激活的路径
  const commonStore = useCommonStore()
  commonStore.setActivePath(activePath)
  // 修改页面标题
  document.title = `${activePath[activePath.length - 1].name} - Drogon Admin`
})

export default router
