import type { LoginResponse, MenuResponse } from './auth'
import type { UploadAvatarResponse, User, UserRole } from './user'
import type { Department } from './department'
import type { Role, RoleDept } from './role'
import { PaginatedResponse } from './common'
import { MenuType } from './enums'
import { Module } from './module'

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
    [0, 1, 2].includes((data as any).type)

  if (!isCommonFieldsValid) {
    return false
  }

  const menuData = data as MenuResponse
  const { type } = menuData

  switch (type) {
    case MenuType.Menu:
      return (
        !isContainKey(menuData, 'path') &&
        !isContainKey(menuData, 'component') &&
        isArrayOrUndefinedField(menuData, 'children', isMenuResponse)
      )
    case MenuType.Page:
      return (
        isNotEmptyStringField(menuData, 'path') &&
        !menuData.path!.startsWith('http://') &&
        !menuData.path!.startsWith('https://') &&
        isNotEmptyStringField(menuData, 'component') &&
        !isContainKey(menuData, 'children')
      )
    case MenuType.OutLink:
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
    isNumberField(data, 'sort_num') &&
    isArrayOrUndefinedField(data, 'children', isDepartment)
  )
}

export function isDeptTree(data: unknown): data is Department[] {
  return isArray(data, isDepartment)
}

export function isRoleDept(data: unknown): data is RoleDept {
  return (
    isObject(data) &&
    isNumberField(data, 'role_id') &&
    isNumberField(data, 'dept_id')
  )
}

export function isRole(data: unknown): data is Role {
  return (
    isObject(data) &&
    isNumberField(data, 'role_id') &&
    isStringField(data, 'name') &&
    isStringField(data, 'code') &&
    isStringOrUndefinedField(data, 'description') &&
    [0, 1, 2].includes((data as any).quota_type) &&
    isNumberOrUndefinedField(data, 'user_quota') &&
    [0, 1, 2].includes((data as any).relation_type) &&
    isArrayOrUndefinedField(data, 'depts', isRoleDept)
  )
}
export function isUserRole(data: unknown): data is UserRole {
  return (
    isObject(data) &&
    isNumberField(data, 'user_id') &&
    isNumberField(data, 'role_id')
  )
}

export function isUser(data: unknown): data is User {
  return (
    isObject(data) &&
    isNumberField(data, 'user_id') &&
    isNotEmptyStringField(data, 'avatar') &&
    isNotEmptyStringField(data, 'username') &&
    isNotEmptyStringField(data, 'nickname') &&
    [0, 1, 2].includes((data as any).sex) &&
    isNumberField(data, 'dept_id') &&
    isStringOrUndefinedField(data, 'phone_number') &&
    isStringOrUndefinedField(data, 'email') &&
    [0, 1].includes((data as any).status) &&
    isArrayOrUndefinedField(data, 'user_roles', isUserRole)
  )
}

export function isPaginatedResponse<T>(
  isItemValid: (item: unknown) => item is T
) {
  return (data: unknown): data is PaginatedResponse<T> => {
    return (
      isObject(data) &&
      isNumberField(data, 'total') &&
      isNumberField(data, 'page') &&
      isNumberField(data, 'page_size') &&
      isArrayField(data, 'list', isItemValid)
    )
  }
}

export function isModule(data: unknown): data is Module {
  return (
    isObject(data) &&
    isNumberField(data, 'module_id') &&
    isStringField(data, 'name') &&
    isStringOrUndefinedField(data, 'description') &&
    isNumberField(data, 'sort_num') &&
    isNumberOrUndefinedField(data, 'parent_id') &&
    isArrayOrUndefinedField(data, 'children', isModule) &&
    isArrayOrUndefinedField(data, 'action_priority', isActionPriority)
  )
}

export function isModuleTree(data: unknown): data is Module[] {
  return isArray(data, isModule)
}

export function isActionPriority(
  data: unknown
): data is { high: number; low: number } {
  return (
    isObject(data) && isNumberField(data, 'high') && isNumberField(data, 'low')
  )
}
