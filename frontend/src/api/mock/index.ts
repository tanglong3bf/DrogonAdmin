import { mock } from '@/utils/request'

// mock
import './auth'
import './dept'
import './user'
import './user_center'
import './role'
import './module'

// 放行未mock的接口
mock.onAny().passThrough()
