import type { LoginResponse, MenuResponse } from './auth'
import type { UploadAvatarResponse } from './user'
import type { Department } from './department'

const isObject = (data: unknown) => typeof data === 'object' && data !== null
const isArray = (data: unknown, callback: any) =>
  Array.isArray(data) && data.every(callback)

const isContainKey = (data: object, key: string) => key in data

const isStringField = (data: object, key: string) =>
  isContainKey(data, key) && typeof (data as any)[key] === 'string'
const isNotEmptyStringField = (data: object, key: string) =>
  isStringField(data, key) && (data as any)[key] !== ''
const isStringOrUndefinedField = (data: object, key: string) =>
  !isContainKey(data, key) || isStringField(data, key)

const isNumberField = (data: object, key: string) =>
  isContainKey(data, key) && typeof (data as any)[key] === 'number'
const isNumberOrUndefinedField = (data: object, key: string) =>
  !isContainKey(data, key) || isNumberField(data, key)

const isArrayField = (data: object, key: string, callback: any) =>
  isContainKey(data, key) && (data as any)[key].every(callback)
const isArrayOrUndefinedField = (data: object, key: string, callback: any) =>
  !isContainKey(data, key) || isArrayField(data, key, callback)

export function isLoginResponse(data: unknown): data is LoginResponse {
  return (
    isObject(data) &&
    isNotEmptyStringField(data, 'token') &&
    isArrayField(data, 'menu_list', isMenuResponse)
  )
}

export function isMenuResponse(data: unknown): data is MenuResponse {
  const isCommonFieldsValid =
    isObject(data) &&
    isNumberField(data, 'menu_id') &&
    isStringOrUndefinedField(data, 'icon') &&
    isStringField(data, 'name') &&
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
        isArrayOrUndefinedField(menuData, 'children', isMenuResponse)
      )
    case 'page':
      return (
        isNotEmptyStringField(menuData, 'path') &&
        !menuData.path!.startsWith('http://') &&
        !menuData.path!.startsWith('https://') &&
        isNotEmptyStringField(menuData, 'component') &&
        !isContainKey(menuData, 'children')
      )
    case 'out_link':
      return (
        isNotEmptyStringField(menuData, 'path') &&
        (menuData.path!.startsWith('http://') ||
          menuData.path!.startsWith('https://')) &&
        !isContainKey(menuData, 'component') &&
        !isContainKey(menuData, 'children')
      )
  }
}
export function isUploadAvatarResponse(
  data: unknown
): data is UploadAvatarResponse {
  return isObject(data) && isNotEmptyStringField(data, 'file_path')
}

export function isDepartment(data: unknown): data is Department {
  return (
    isObject(data) &&
    isNumberField(data, 'dept_id') &&
    isStringField(data, 'name') &&
    isNumberOrUndefinedField(data, 'parent_id') &&
    isNumberField(data, 'order_no') &&
    isArrayOrUndefinedField(data, 'children', isDepartment)
  )
}

export function isDeptTree(data: unknown): data is Department[] {
  return isArray(data, isDepartment)
}
