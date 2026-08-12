import { mock } from '@/utils/request'
import { Random } from 'mockjs'
import type { ResponseBody } from '@/types/common'
import type { LoginResponse } from '@/types/auth'
import mockConfig from './mock.config.json'
import { AxiosRequestConfig } from 'axios'

/**
 * 登录接口mock
 */
if (mockConfig.auth.login) {
  mock
    .onPost('/login')
    .reply(
      (config: AxiosRequestConfig): [number, ResponseBody<LoginResponse>] => {
        console.log(config)
        if (config.data === undefined || config.data.length === 0) {
          return [401, { code: -1, error: '用户名或密码错误' }]
        }
        const { username, password } = JSON.parse(config.data)
        if (username !== 'admin123' || password !== '123456') {
          return [401, { code: -1, error: '用户名或密码错误' }]
        }
        return [
          200,
          {
            data: {
              // 前端处理只在乎有没有token，不在意token内容
              token: 'abcdefghijklmn',
              user_info: {
                user_id: 1,
                username: 'admin123',
                nickname: '超级管理员',
                avatar: Random.image('150x150', '#888', '#FFF', 'avatar'),
                sex: 1,
                deparemnt_id: 1,
                roles_id: [1],
                phone_number: '13388886666',
                email: 'admin123@dr-admin.com',
                status: 'normal'
              },
              menu_list: [
                {
                  menu_id: 1,
                  icon: 'ion:settings-sharp',
                  name: '系统管理',
                  type: 0,
                  children: [
                    {
                      menu_id: 2,
                      icon: 'ion:settings-sharp',
                      name: '组织架构',
                      type: 0,
                      children: [
                        {
                          menu_id: 5,
                          path: '/sys/org/dept',
                          icon: 'mingcute:department-fill',
                          name: '部门管理',
                          component: '/sys/org/Dept',
                          type: 1
                        },
                        {
                          menu_id: 6,
                          path: '/sys/org/user',
                          icon: 'ri:user-fill',
                          name: '用户管理',
                          component: '/sys/org/User',
                          type: 1
                        }
                      ]
                    },
                    {
                      menu_id: 3,
                      icon: 'ion:settings-sharp',
                      name: '权限管理',
                      type: 0,
                      children: [
                        {
                          menu_id: 7,
                          path: '/sys/authorization/role',
                          icon: 'fa7-solid:user-cog',
                          name: '角色管理',
                          component: '/sys/authorization/Role',
                          type: 1
                        },
                        {
                          menu_id: 8,
                          path: '/sys/authorization/permission',
                          icon: 'ix:user-key',
                          name: '权限分配',
                          component: '/sys/authorization/Permission',
                          type: 1
                        }
                      ]
                    },
                    {
                      menu_id: 4,
                      icon: 'ion:settings-sharp',
                      name: '资源管理',
                      type: 0,
                      children: [
                        {
                          menu_id: 9,
                          path: '/sys/resources/module',
                          icon: 'tdesign:module-filled',
                          name: '模块管理',
                          component: '/sys/resources/Module',
                          type: 1
                        },
                        {
                          menu_id: 10,
                          path: '/sys/resources/menu',
                          icon: 'fluent-mdl2:context-menu',
                          name: '菜单管理',
                          component: '/sys/resources/Menu',
                          type: 1
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
                  type: 2
                }
              ]
            }
          }
        ]
      }
    )
}
