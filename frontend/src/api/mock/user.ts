import { Random, mock as RawMock } from 'mockjs'
import { mock } from '@/utils/request'
import mockConfig from './mock.config.json'
import type { AxiosRequestConfig } from 'axios'
import { PaginatedResponse, ResponseBody } from '@/types/common'
import { UploadAvatarResponse, User } from '@/types/user'

/**
 * 更新用户基础信息mock
 */
if (mockConfig.user.update_basic_info) {
  mock
    .onPut(/^http:\/\/localhost:8000\/user$/)
    .reply((config: AxiosRequestConfig): [number, ResponseBody<any>?] => {
      console.log(config)
      if (config.data === undefined || config.data.length === 0) {
        return [400, { code: -1, error: '请求体缺少要修改的数据' }]
      }
      const { nickname, sex, phone_number, email } = JSON.parse(config.data)
      if (
        nickname === undefined &&
        sex === undefined &&
        phone_number === undefined &&
        email === undefined
      ) {
        return [400, { code: -1, error: '请求体缺少要修改的数据' }]
      }
      return [204]
    })
}

/**
 * 更新用户密码mock
 */
if (mockConfig.user.update_password) {
  mock
    .onPut(/^http:\/\/localhost:8000\/user\/password$/)
    .reply((config: AxiosRequestConfig): [number, ResponseBody<any>?] => {
      console.log(config)
      if (config.data === undefined || config.data.length === 0) {
        return [400, { code: -1, error: '请求体缺少参数' }]
      }
      const { oldPassword, newPassword } = JSON.parse(config.data)
      if (oldPassword === undefined || newPassword === undefined) {
        return [400, { code: -1, error: '请求体缺少必备参数' }]
      }
      return [204]
    })
}

let times = 0
/**
 * 上传用户头像mock
 */
if (mockConfig.user.upload_avatar) {
  mock
    .onPost(/^http:\/\/localhost:8000\/user\/upload_avatar$/)
    .reply(
      (
        config: AxiosRequestConfig
      ): [number, ResponseBody<UploadAvatarResponse>] => {
        console.log(config)
        return [
          200,
          {
            data: {
              file_path: Random.image(
                '150x150',
                '#888',
                '#FFF',
                `avatar ${times++}`
              )
            }
          }
        ]
      }
    )
}

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

if (mockConfig.user.new_user) {
  mock.onPost('/user').reply((config: AxiosRequestConfig): [number] => {
    const data = JSON.parse(config.data)
    console.log('新增用户', data)

    return [201]
  })
}
