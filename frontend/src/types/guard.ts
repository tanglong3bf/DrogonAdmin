import type { LoginResponse, MenuResponse } from './auth'

const isObject = (data: unknown) => typeof data === 'object' && data !== null
const isContainKey = (data: object, key: string) => key in data

const isString = (data: object, key: string) =>
  isContainKey(data, key) && typeof (data as any)[key] === 'string'
const isNotEmptyString = (data: object, key: string) =>
  isString(data, key) && (data as any)[key] !== ''
const isStringOrUndefined = (data: object, key: string) =>
  !isContainKey(data, key) || isString(data, key)

const isNumber = (data: object, key: string) =>
  isContainKey(data, key) && typeof (data as any)[key] === 'number'

const isArray = (data: object, key: string, callback: any) =>
  isContainKey(data, key) && (data as any)[key].every(callback)
const isArrayOrUndefined = (data: object, key: string, callback: any) =>
  !isContainKey(data, key) || isArray(data, key, callback)

export function isLoginResponse(data: unknown): data is LoginResponse {
  return (
    isObject(data) &&
    isNotEmptyString(data, 'token') &&
    isArray(data, 'menu_list', isMenuResponse)
  )
}

export function isMenuResponse(data: unknown): data is MenuResponse {
  const isCommonFieldsValid =
    isObject(data) &&
    isNumber(data, 'menu_id') &&
    isStringOrUndefined(data, 'icon') &&
    isString(data, 'name') &&
    ['menu', 'page', 'out_link'].includes((data as any).type)

  if (!isCommonFieldsValid) {
    return false
  }

  const menuData = data as MenuResponse
  const { type } = menuData

  switch (type) {
    case 'menu':
      return (
        !isContainKey(menuData, 'path') &&
        !isContainKey(menuData, 'component') &&
        isArrayOrUndefined(menuData, 'children', isMenuResponse)
      )
    case 'page':
      return (
        isNotEmptyString(menuData, 'path') &&
        !menuData.path!.startsWith('http://') &&
        !menuData.path!.startsWith('https://') &&
        isNotEmptyString(menuData, 'component') &&
        !isContainKey(menuData, 'children')
      )
    case 'out_link':
      return (
        isNotEmptyString(menuData, 'path') &&
        (menuData.path!.startsWith('http://') ||
          menuData.path!.startsWith('https://')) &&
        !isContainKey(menuData, 'component') &&
        !isContainKey(menuData, 'children')
      )
  }
}
