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
                  role_id: 1,
                  name: '管理员',
                  code: 'admin',
                  description: '拥有系统的最高权限',
                  user_quota: 1,
                  quota_type: 'TotalLimit'
                },
                {
                  role_id: 2,
                  name: '普通用户',
                  code: 'user',
                  description: '拥有系统的基本权限',
                  quota_type: 'Unlimited'
                }
              ]
            }
          }
        ]
      }
    )
}
