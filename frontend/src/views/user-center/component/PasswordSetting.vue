<script setup lang="ts">
import { FormInstance, FormRules } from 'element-plus/es'
import { reactive, ref } from 'vue'
import type {
  InternalRuleItem,
  Value
} from 'async-validator/dist-types/interface.d.ts'
import { UserPasswordUpdateRequest } from '@/types/user'
import { updatePassword } from '@/api/user'

/**
 * 更新密码表单数据
 */
interface PasswordModifyForm {
  oldPassword: string
  oldPassword2: string
  newPassword: string
}

/**
 * 表单实体的引用
 */
const formRef = ref<FormInstance>()

/**
 * 表单数据
 */
const formData = reactive<PasswordModifyForm>({
  oldPassword: '',
  oldPassword2: '',
  newPassword: ''
})

/**
 * 验证两次输入的旧密码相同
 */
const validateOldPwdEqual = (
  _rule: InternalRuleItem,
  value: Value,
  callback: (error?: string | Error) => void
) => {
  const { oldPassword } = formData

  if (!value) {
    return callback(new Error('请再次确认旧密码'))
  }

  if (value !== oldPassword) {
    callback(new Error('两次输入的旧密码不一致，请重新输入'))
  } else {
    callback()
  }
}

/**
 * 验证新密码和旧密码不同
 */
const validateNewPwdNotEqualOld = (
  _rule: InternalRuleItem,
  value: Value,
  callback: (error?: string | Error) => void
) => {
  const { oldPassword, oldPassword2 } = formData

  if (!value) {
    return callback(new Error('请输入新密码'))
  }

  if (oldPassword && oldPassword2 && oldPassword === oldPassword2) {
    if (value === oldPassword) {
      callback(new Error('新密码不能与旧密码相同，请重新输入'))
    } else {
      callback()
    }
  } else {
    callback(new Error('请先确保两次输入的旧密码一致且不为空'))
  }
}

/**
 * 表单校验规则
 */
const rules = reactive<FormRules<PasswordModifyForm>>({
  oldPassword: [
    { required: true, message: '请填写旧密码', trigger: 'blur' },
    { min: 6, max: 20, message: '旧密码长度不合法', trigger: 'blur' }
  ],
  oldPassword2: [
    { required: true, message: '请填写旧密码', trigger: 'blur' },
    { min: 6, max: 20, message: '旧密码长度不合法', trigger: 'blur' },
    { validator: validateOldPwdEqual, trigger: 'blur' }
  ],
  newPassword: [
    { required: true, message: '请填写新密码', trigger: 'blur' },
    { min: 6, max: 20, message: '新密码长度不合法', trigger: 'blur' },
    { validator: validateNewPwdNotEqualOld, trigger: 'blur' }
  ]
})

/**
 * 检查数据是否发生变化，且满足长度规则
 */
const checkUpdatable = (): boolean => {
  const oldPasswordLength = formData.oldPassword.length
  const newPasswordLength = formData.newPassword.length

  return (
    oldPasswordLength >= 6 &&
    oldPasswordLength <= 20 &&
    newPasswordLength >= 6 &&
    newPasswordLength <= 20 &&
    formData.oldPassword === formData.oldPassword2 &&
    formData.newPassword !== formData.oldPassword
  )
}

/**
 * 更新密码按钮，校验表单数据并提交请求
 */
const updatePasswordBtn = async (formEl?: FormInstance) => {
  const isValid = formEl?.validate()
  if (isValid) {
    const request: UserPasswordUpdateRequest = {
      oldPassword: formData.oldPassword,
      newPassword: formData.newPassword
    }

    await updatePassword(request)
  }
}
</script>

<template>
  <el-form
    :model="formData"
    :rules="rules"
    label-width="auto"
    label-position="top"
    ref="formRef"
  >
    <el-form-item label="旧密码" prop="oldPassword">
      <el-input
        type="password"
        v-model="formData.oldPassword"
        placeholder="请输入旧密码"
      />
    </el-form-item>
    <el-form-item label="旧密码（确认）" prop="oldPassword2">
      <el-input
        type="password"
        v-model="formData.oldPassword2"
        placeholder="请再次输入旧密码"
      />
    </el-form-item>
    <el-form-item label="新密码" prop="newPassword">
      <el-input
        type="password"
        v-model="formData.newPassword"
        placeholder="请输入新密码"
      />
    </el-form-item>
  </el-form>
  <div class="update-btn">
    <el-button :disabled="!checkUpdatable()" @click="updatePasswordBtn(formRef)"
      >修改密码</el-button
    >
  </div>
</template>

<style lang="scss" scoped>
.update-btn {
  display: flex;
  flex-direction: row-reverse;
}
</style>
