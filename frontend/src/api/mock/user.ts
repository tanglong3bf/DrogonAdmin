import { Random } from 'mockjs'
import { mock } from '@/utils/request'
import mockConfig from './mock.config.json'
import type { AxiosRequestConfig } from 'axios'
import { ResponseBody } from '@/types/common'
import { UploadAvatarResponse } from '@/types/user'

/**
 * 更新用户基础信息mock
 */
if (mockConfig.user.update_basic_info) {
  mock
    .onPut(/^http:\/\/localhost:8000\/user$/)
    .reply((config: AxiosRequestConfig): [number, ResponseBody<any>?] => {
      console.log(config)
      if (config.data === undefined || config.data.empty()) {
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
      if (config.data === undefined || config.data.empty()) {
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
