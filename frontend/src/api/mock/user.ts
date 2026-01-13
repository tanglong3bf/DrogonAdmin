import { mock } from 'mockjs'
import mockConfig from './mock.config.json'

/**
 * 更新用户基础信息mock
 */
if (mockConfig.user.update_basic_info) {
  mock(
    /^http:\/\/localhost:8000\/user\/[1-9]\d*$/,
    'put',
    (options): undefined => {
      console.log(options)
    }
  )
}

/**
 * 更新用户密码mock
 */
if (mockConfig.user.update_password) {
  mock(
    /^http:\/\/localhost:8000\/user\/[1-9]\d*\/password$/,
    'put',
    (options): undefined => {
      console.log(options)
    }
  )
}
