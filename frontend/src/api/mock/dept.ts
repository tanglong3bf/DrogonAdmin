import { mock } from '@/utils/request'
import mockConfig from './mock.config.json'
import type { AxiosRequestConfig } from 'axios'
import { ResponseBody } from '@/types/common'

/**
 * 获取部门树接口mock
 */
if (mockConfig.dept.get_dept_tree) {
  mock
    .onGet(/^\/dept\/tree$/)
    .reply((config: AxiosRequestConfig): [number, ResponseBody<any>?] => {
      console.log(config)
      return [
        200,
        {
          data: [
            {
              children: [
                {
                  dept_id: 2,
                  name: '人事部',
                  order_no: 1,
                  parent_id: 1
                },
                {
                  dept_id: 3,
                  name: '财务部',
                  order_no: 2,
                  parent_id: 1
                },
                {
                  children: [
                    {
                      dept_id: 5,
                      name: 'xx项目开发组',
                      order_no: 1,
                      parent_id: 4
                    },
                    {
                      dept_id: 6,
                      name: 'yy项目开发组',
                      order_no: 2,
                      parent_id: 4
                    }
                  ],
                  dept_id: 4,
                  name: '技术部',
                  order_no: 3,
                  parent_id: 1
                }
              ],
              dept_id: 1,
              name: '钱途无量有限公司',
              order_no: 1
            },
            {
              dept_id: 7,
              name: '分公司',
              order_no: 2
            }
          ]
        }
      ]
    })
}

/**
 * 新增部门接口mock
 */
if (mockConfig.dept.new_dept) {
  mock.onPost(/^\/dept$/).reply((config: AxiosRequestConfig): [number] => {
    console.log(config)
    return [201]
  })
}

/**
 * 更新部门接口mock
 */
if (mockConfig.dept.update_dept) {
  mock
    .onPut(/^\/dept\/[1-9]\d*$/)
    .reply((config: AxiosRequestConfig): [number] => {
      console.log(config)
      return [204]
    })
}

/**
 * 删除部门接口mock
 */
if (mockConfig.dept.delete_dept) {
  mock
    .onDelete(/^\/dept\/[1-9]\d*$/)
    .reply((config: AxiosRequestConfig): [number] => {
      console.log(config)
      return [204]
    })
}

/**
 * 排序部门接口mock
 */
if (mockConfig.dept.sort_dept) {
  mock.onPut('/dept/sort').reply((config: AxiosRequestConfig): [number] => {
    console.log(config)
    return [204]
  })
}
