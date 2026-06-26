import { mock as RawMock } from 'mockjs'
import { mock } from '@/utils/request'
import mockConfig from './mock.config.json'
import type { AxiosRequestConfig } from 'axios'
import { PaginatedResponse, ResponseBody } from '@/types/common'
import { User } from '@/types/user'

/**
 * 获取用户列表mock
 */
if (mockConfig.user.get_user_list) {
  mock
    .onGet('/user')
    .reply(
      (
        config: AxiosRequestConfig
      ): [number, ResponseBody<PaginatedResponse<User>>] => {
        console.log(config)

        const userList = RawMock({
          'list|10': [
            {
              'user_id|+1': 1, // 从 1 开始递增
              avatar: '@image("150x150", "#888", "#FFF", "avatar")',
              email: '@email()',
              'dept_id|1-5': 1, // 随机部门 ID 1~5
              username: '@string("lower", 6, 12)',
              nickname: '@cname()',
              phone_number: /^1[3-9]\d{9}$/,
              'sex|1': [0, 1, 2],
              'status|1': [0, 1]
            }
          ]
        }).list

        return [
          200,
          {
            data: {
              total: 100,
              page: 1,
              page_size: 10,
              list: userList
            }
          }
        ]
      }
    )
}

/**
 * 新增用户mock
 */
if (mockConfig.user.new_user) {
  mock.onPost('/user').reply((config: AxiosRequestConfig): [number] => {
    const data = JSON.parse(config.data)
    console.log('新增用户', data)

    return [201]
  })
}

/**
 * 更新用户mock
 */
if (mockConfig.user.update_user) {
  mock
    .onPatch(/^\/user\/[1-9]\d*$/)
    .reply((config: AxiosRequestConfig): [number] => {
      const data = JSON.parse(config.data)
      console.log('更新用户', data)

      return [204]
    })
}

/**
 * 删除用户mock
 */
if (mockConfig.user.delete_user) {
  mock
    .onDelete(/^\/user\/[1-9]\d*$/)
    .reply((config: AxiosRequestConfig): [number] => {
      console.log('删除用户', config)
      return [204]
    })
}
