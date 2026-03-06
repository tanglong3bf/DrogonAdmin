import { mock } from '@/utils/request'

// mock
import './auth'
import './dept'
import './user'

// 放行未mock的接口
mock.onAny().passThrough()
