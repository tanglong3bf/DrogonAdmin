import { mock } from '@/utils/request'
import mockConfig from './mock.config.json'
import type { AxiosRequestConfig } from 'axios'
import { ResponseBody } from '@/types/common'
import { Menu } from '@/types/menu'

/**
 * 获取菜单树接口mock
 */
if (mockConfig.menu.get_menu_tree) {
  mock
    .onGet('/menu/tree')
    .reply((config: AxiosRequestConfig): [number, ResponseBody<Menu[]>?] => {
      console.log(config)
      return [
        200,
        {
          data: [
            {
              menu_id: 1,
              index: '1',
              icon: 'ion:settings-sharp',
              name: '系统管理',
              type: 0, // menu
              sort_num: 0,
              version: 0,
              children: [
                {
                  menu_id: 2,
                  index: '2',
                  icon: 'ion:settings-sharp',
                  name: '组织架构',
                  type: 0, // menu
                  parent_id: 1,
                  sort_num: 0,
                  version: 0,
                  children: [
                    {
                      menu_id: 5,
                      index: '5',
                      path: '/sys/org/dept',
                      icon: 'mingcute:department-fill',
                      name: '部门管理',
                      component: '/sys/org/Dept',
                      type: 1, // page
                      parent_id: 2,
                      sort_num: 0,
                      version: 0
                    },
                    {
                      menu_id: 6,
                      index: '6',
                      path: '/sys/org/user',
                      icon: 'ri:user-fill',
                      name: '用户管理',
                      component: '/sys/org/User',
                      type: 1, // page
                      parent_id: 2,
                      sort_num: 1,
                      version: 0
                    }
                  ]
                },
                {
                  menu_id: 3,
                  index: '3',
                  icon: 'ion:settings-sharp',
                  name: '权限管理',
                  type: 0, // menu
                  parent_id: 1,
                  sort_num: 1,
                  version: 0,
                  children: [
                    {
                      menu_id: 7,
                      index: '7',
                      path: '/sys/authorization/role',
                      icon: 'fa7-solid:user-cog',
                      name: '角色管理',
                      component: '/sys/authorization/Role',
                      type: 1, // page
                      parent_id: 3,
                      sort_num: 0,
                      version: 0
                    },
                    {
                      menu_id: 8,
                      index: '8',
                      path: '/sys/authorization/permission',
                      icon: 'ix:user-key',
                      name: '权限分配',
                      component: '/sys/authorization/Permission',
                      type: 1, // page
                      parent_id: 3,
                      sort_num: 1,
                      version: 0
                    }
                  ]
                },
                {
                  menu_id: 4,
                  index: '4',
                  icon: 'ion:settings-sharp',
                  name: '资源管理',
                  type: 0, // menu
                  parent_id: 1,
                  sort_num: 2,
                  version: 0,
                  children: [
                    {
                      menu_id: 9,
                      index: '9',
                      path: '/sys/resources/module',
                      icon: 'tdesign:module-filled',
                      name: '模块管理',
                      component: '/sys/resources/Module',
                      type: 1, // page
                      parent_id: 4,
                      sort_num: 0,
                      version: 0
                    },
                    {
                      menu_id: 10,
                      index: '10',
                      path: '/sys/resources/menu',
                      icon: 'fluent-mdl2:context-menu',
                      name: '菜单管理',
                      component: '/sys/resources/Menu',
                      type: 1, // page
                      parent_id: 4,
                      sort_num: 1,
                      version: 0
                    }
                  ]
                }
              ]
            },
            {
              menu_id: 11,
              index: '11',
              path: 'https://github.com/tanglong3bf/DrogonAdmin',
              icon: 'octicon:mark-github-24',
              name: '源码地址',
              type: 2, // out_link
              sort_num: 1,
              version: 0
            }
          ]
        }
      ]
    })
}

/**
 * 新增菜单接口mock
 */
if (mockConfig.menu.new_menu) {
  mock.onPost('/menu').reply((config: AxiosRequestConfig): [number] => {
    console.log(config)
    return [201]
  })
}

/**
 * 更新菜单接口mock
 */
if (mockConfig.menu.update_menu) {
  mock
    .onPatch(/^\/menu\/[1-9]\d*$/)
    .reply((config: AxiosRequestConfig): [number] => {
      console.log(config)
      return [204]
    })
}

/**
 * 删除菜单接口mock
 */
if (mockConfig.menu.delete_menu) {
  mock
    .onDelete(/^\/menu\/[1-9]\d*(\?.*)?$/)
    .reply((config: AxiosRequestConfig): [number] => {
      console.log(config)
      return [204]
    })
}

/**
 * 排序菜单接口mock
 */
if (mockConfig.menu.sort_menu) {
  mock.onPost('/menu/sort').reply((config: AxiosRequestConfig): [number] => {
    console.log(config)
    return [204]
  })
}
