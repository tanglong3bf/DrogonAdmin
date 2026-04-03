import { mock } from '@/utils/request'
import mockConfig from './mock.config.json'
import type { AxiosRequestConfig } from 'axios'
import { PaginatedResponse, ResponseBody } from '@/types/common'
import { Role } from '@/types/role'

/**
 * 获取角色列表mock
 */
if (mockConfig.role.get_role_list) {
  mock
    .onGet('/role')
    .reply(
      (
        config: AxiosRequestConfig
      ): [number, ResponseBody<PaginatedResponse<Role>>] => {
        console.log(config)
        return [
          200,
          {
            data: {
              page: 1,
              page_size: 10,
              total: 2,
              list: [
                {
                  code: 'admin',
                  description: 'aaa',
                  name: '系统管理员',
                  quota_type: 1,
                  role_id: 1,
                  user_quota: 3,
                  relation_type: 1,
                  depts: [
                    { role_id: 1, dept_id: 1 },
                    { role_id: 1, dept_id: 7 }
                  ]
                },
                {
                  code: 'test_role',
                  description: 'testtesttest',
                  name: '测试角色',
                  quota_type: 2,
                  role_id: 2,
                  user_quota: 3,
                  relation_type: 0
                },
                {
                  code: 'test_role222',
                  description: 'testtesttest222',
                  name: '测试角色222',
                  quota_type: 2,
                  role_id: 3,
                  user_quota: 3,
                  relation_type: 2,
                  depts: [
                    { role_id: 3, dept_id: 4 },
                    { role_id: 3, dept_id: 5 },
                    { role_id: 3, dept_id: 6 }
                  ]
                }
              ]
            }
          }
        ]
      }
    )
}

if (mockConfig.role.new_role) {
  mock.onPost('/role').reply((config: AxiosRequestConfig): [number] => {
    console.log(config)
    return [201]
  })
}

if (mockConfig.role.update_role) {
  mock
    .onPut(/^\/role\/[1-9]\d*$/)
    .reply((config: AxiosRequestConfig): [number] => {
      console.log(config)
      return [204]
    })
}

if (mockConfig.role.delete_role) {
  mock
    .onDelete(/^\/role\/[1-9]\d*$/)
    .reply((config: AxiosRequestConfig): [number] => {
      console.log(config)
      return [204]
    })
}
