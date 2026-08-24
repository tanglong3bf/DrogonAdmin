import { mock } from '@/utils/request'
import mockConfig from './mock.config.json'
import type { AxiosRequestConfig } from 'axios'
import { ResponseBody } from '@/types/common'
import { Module } from '@/types/module'

const actions = [
  [],
  [],
  [],
  [
    {
      action_id: 1,
      code: 'dept:view',
      has_data_permission: false,
      module_id: 3,
      name: '浏览部门管理页面',
      sort_num: 0,
      version: 0
    },
    {
      action_id: 2,
      code: 'dept:query',
      has_data_permission: true,
      module_id: 3,
      name: '查询部门树',
      sort_num: 1,
      version: 0
    },
    {
      action_id: 3,
      code: 'dept:create',
      has_data_permission: true,
      module_id: 3,
      name: '新增部门',
      sort_num: 2,
      version: 0
    },
    {
      action_id: 4,
      code: 'dept:sort',
      has_data_permission: true,
      module_id: 3,
      name: '排序部门',
      sort_num: 3,
      version: 0
    },
    {
      action_id: 5,
      code: 'dept:update',
      has_data_permission: true,
      module_id: 3,
      name: '更新部门',
      sort_num: 4,
      version: 0
    },
    {
      action_id: 6,
      code: 'dept:delete',
      has_data_permission: true,
      module_id: 3,
      name: '删除部门',
      sort_num: 5,
      version: 0
    }
  ],
  [
    {
      action_id: 7,
      code: 'user:view',
      has_data_permission: false,
      module_id: 4,
      name: '浏览用户管理页面',
      sort_num: 0,
      version: 0
    },
    {
      action_id: 8,
      code: 'user:query',
      has_data_permission: true,
      module_id: 4,
      name: '查询用户',
      sort_num: 1,
      version: 0
    },
    {
      action_id: 9,
      code: 'user:create',
      has_data_permission: true,
      module_id: 4,
      name: '新增用户',
      sort_num: 2,
      version: 0
    },
    {
      action_id: 10,
      code: 'user:update',
      has_data_permission: true,
      module_id: 4,
      name: '更新用户',
      sort_num: 3,
      version: 0
    },
    {
      action_id: 11,
      code: 'user:delete',
      has_data_permission: true,
      module_id: 4,
      name: '删除用户',
      sort_num: 4,
      version: 0
    }
  ],
  [],
  [
    {
      action_id: 12,
      code: 'role:view',
      has_data_permission: false,
      module_id: 6,
      name: '浏览角色管理页面',
      sort_num: 0,
      version: 0
    },
    {
      action_id: 13,
      code: 'role:query',
      has_data_permission: false,
      module_id: 6,
      name: '查询角色',
      sort_num: 1,
      version: 0
    },
    {
      action_id: 14,
      code: 'role:create',
      has_data_permission: false,
      module_id: 6,
      name: '新增角色',
      sort_num: 2,
      version: 0
    },
    {
      action_id: 15,
      code: 'role:update',
      has_data_permission: false,
      module_id: 6,
      name: '更新角色',
      sort_num: 3,
      version: 0
    },
    {
      action_id: 16,
      code: 'role:delete',
      has_data_permission: false,
      module_id: 6,
      name: '删除角色',
      sort_num: 4,
      version: 0
    }
  ]
]

const actionPriority = [
  [],
  [],
  [],
  [
    {
      high_id: 2,
      low_id: 3
    },
    {
      high_id: 2,
      low_id: 4
    },
    {
      high_id: 2,
      low_id: 5
    },
    {
      high_id: 5,
      low_id: 6
    }
  ],
  [
    {
      high_id: 7,
      low_id: 8
    },
    {
      high_id: 8,
      low_id: 9
    },
    {
      high_id: 8,
      low_id: 10
    },
    {
      high_id: 10,
      low_id: 11
    }
  ],
  [],
  [
    {
      high_id: 12,
      low_id: 13
    },
    {
      high_id: 13,
      low_id: 14
    },
    {
      high_id: 13,
      low_id: 15
    },
    {
      high_id: 15,
      low_id: 16
    }
  ]
]

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
              children: [
                {
                  children: [
                    {
                      actions: actions[3],
                      priorities: actionPriority[3],
                      module_id: 3,
                      name: '部门管理',
                      parent_id: 2,
                      sort_num: 0,
                      version: 0
                    },
                    {
                      actions: actions[4],
                      priorities: actionPriority[4],
                      module_id: 4,
                      name: '用户管理',
                      parent_id: 2,
                      sort_num: 1,
                      version: 0
                    }
                  ],
                  module_id: 2,
                  name: '系统管理',
                  parent_id: 1,
                  sort_num: 0,
                  version: 0
                },
                {
                  children: [
                    {
                      actions: actions[6],
                      priorities: actionPriority[6],
                      module_id: 6,
                      name: '角色管理',
                      parent_id: 5,
                      sort_num: 0,
                      version: 0
                    },
                    {
                      module_id: 7,
                      name: '权限分配',
                      parent_id: 5,
                      sort_num: 1,
                      version: 0
                    }
                  ],
                  module_id: 5,
                  name: '权限管理',
                  parent_id: 1,
                  sort_num: 1,
                  version: 0
                }
              ],
              module_id: 1,
              name: '系统管理',
              sort_num: 0,
              version: 0
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

/**
 * 排序模块接口mock
 */
if (mockConfig.module.sort_module) {
  mock.onPost('/module/sort').reply((config: AxiosRequestConfig): [number] => {
    console.log(config)
    return [204]
  })
}

/**
 * 分配功能接口mock
 */
if (mockConfig.module.assign_actions) {
  mock
    .onPost(/^\/module\/[1-9]\d*\/actions$/)
    .reply((config: AxiosRequestConfig): [number] => {
      console.log(config)
      return [204]
    })
}
