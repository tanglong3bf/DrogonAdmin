import { mock } from 'mockjs'
import type { ResponseBody } from '@/types/common'
import type { LoginResponse } from '@/types/auth'
import mockConfig from './mock.config.json'

/**
 * 登录接口mock
 */
if (mockConfig.auth.login) {
  mock(
    'http://localhost:8000/login',
    'post',
    (options): ResponseBody<LoginResponse> => {
      console.log(options)
      return {
        data: {
          // 前端处理只在乎有没有token，不在意token内容
          token: 'abcdefghijklmn',
          menu_list: [
            {
              menu_id: 1,
              icon: 'ion:settings-sharp',
              name: '系统管理',
              type: 'menu',
              children: [
                {
                  menu_id: 2,
                  icon: 'ion:settings-sharp',
                  name: '组织架构',
                  type: 'menu',
                  children: [
                    {
                      menu_id: 5,
                      path: '/sys/organization/dept',
                      icon: 'mingcute:department-fill',
                      name: '部门管理',
                      component: '/sys/organization/Dept',
                      type: 'page'
                    },
                    {
                      menu_id: 6,
                      path: '/sys/organization/user',
                      icon: 'ri:user-fill',
                      name: '用户管理',
                      component: '/sys/organization/User',
                      type: 'page'
                    }
                  ]
                },
                {
                  menu_id: 3,
                  icon: 'ion:settings-sharp',
                  name: '模块菜单',
                  type: 'menu',
                  children: [
                    {
                      menu_id: 7,
                      path: '/sys/module_menu/menu',
                      icon: 'tdesign:module-filled',
                      name: '模块管理',
                      component: '/sys/module_menu/Module',
                      type: 'page'
                    },
                    {
                      menu_id: 8,
                      path: '/sys/module_menu/module',
                      icon: 'fluent-mdl2:context-menu',
                      name: '菜单管理',
                      component: '/sys/module_menu/Menu',
                      type: 'page'
                    }
                  ]
                },
                {
                  menu_id: 4,
                  icon: 'ion:settings-sharp',
                  name: '角色权限',
                  type: 'menu',
                  children: [
                    {
                      menu_id: 9,
                      path: '/sys/role_auth/role',
                      icon: 'fa7-solid:user-cog',
                      name: '角色管理',
                      component: '/sys/role_auth/Role',
                      type: 'page'
                    },
                    {
                      menu_id: 10,
                      path: '/sys/role_auth/auth',
                      icon: 'ix:user-key',
                      name: '权限管理',
                      component: '/sys/role_auth/Auth',
                      type: 'page'
                    }
                  ]
                }
              ]
            },
            {
              menu_id: 11,
              path: 'https://github.com/tanglong3bf/DrogonAdmin',
              icon: 'octicon:mark-github-24',
              name: '源码地址',
              type: 'out_link'
            }
          ]
        }
      }
    }
  )
}
