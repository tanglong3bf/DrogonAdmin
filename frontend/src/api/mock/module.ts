import { mock } from '@/utils/request'
import mockConfig from './mock.config.json'
import type { AxiosRequestConfig } from 'axios'
import { ResponseBody } from '@/types/common'
import { Module } from '@/types/module'

/**
 * 获取模块树接口mock
 */
if (mockConfig.module.get_module_tree) {
  mock
    .onGet('/module/tree')
    .reply((config: AxiosRequestConfig): [number, ResponseBody<Module[]>?] => {
      console.log(config)
      return [
        200,
        {
          data: [
            {
              module_id: 1,
              name: '组织架构',
              sort_num: 0,
              children: [
                {
                  module_id: 2,
                  name: '部门管理',
                  sort_num: 0,
                  parent_id: 1
                },
                {
                  module_id: 3,
                  name: '用户管理',
                  sort_num: 1,
                  parent_id: 1
                }
              ]
            },
            {
              module_id: 4,
              name: '权限管理',
              sort_num: 1,
              children: [
                {
                  module_id: 5,
                  name: '角色管理',
                  sort_num: 0,
                  parent_id: 1
                },
                {
                  module_id: 6,
                  name: '权限分配',
                  sort_num: 1,
                  parent_id: 1
                }
              ]
            },
            {
              module_id: 7,
              name: '资源管理',
              sort_num: 2,
              children: [
                {
                  module_id: 8,
                  name: '模块管理',
                  sort_num: 0,
                  parent_id: 1
                },
                {
                  module_id: 9,
                  name: '菜单管理',
                  sort_num: 1,
                  parent_id: 1
                }
              ]
            }
          ]
        }
      ]
    })
}

/**
 * 新增模块接口mock
 */
if (mockConfig.module.new_module) {
  mock.onPost('/module').reply((config: AxiosRequestConfig): [number] => {
    console.log(config)
    return [201]
  })
}

/**
 * 更新模块接口mock
 */
if (mockConfig.module.update_module) {
  mock
    .onPatch(/^\/module\/[1-9]\d*$/)
    .reply((config: AxiosRequestConfig): [number] => {
      console.log(config)
      return [204]
    })
}

/**
 * 删除模块接口mock
 */
if (mockConfig.module.delete_module) {
  mock
    .onDelete(/^\/module\/[1-9]\d*$/)
    .reply((config: AxiosRequestConfig): [number] => {
      console.log(config)
      return [204]
    })
}
