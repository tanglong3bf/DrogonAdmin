import { mock } from '@/utils/request'
import mockConfig from './mock.config.json'
import type { AxiosRequestConfig } from 'axios'
import { ResponseBody } from '@/types/common'

interface Module {
  module_id: number // 模块ID
  name: string // 模块名称
  description?: string // 模块描述
  sort_num: number // 模块排序
  parent_id?: number // 父级ID
  children?: Module[] // 子模块
  child_count?: number // 子模块数量
}
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
