<script setup lang="ts">
import { ElMessage, FormInstance, FormRules } from 'element-plus/es'
import { reactive, ref } from 'vue'
import type {
  InternalRuleItem,
  Value
} from 'async-validator/dist-types/interface.d.ts'
import { UserPasswordUpdateRequest } from '@/types/user'
import { changePassword } from '@/api/user_center'

/**
 * 更新密码表单数据
 */
interface PasswordModifyForm {
  old_password: string
  old_password2: string
  new_password: string
}

/**
 * 表单实体的引用
 */
const formRef = ref<FormInstance>()

/**
 * 表单数据
 */
const formData = reactive<PasswordModifyForm>({
  old_password: '',
  old_password2: '',
  new_password: ''
})

/**
 * 验证两次输入的旧密码相同
 */
const validateOldPwdEqual = (
  _rule: InternalRuleItem,
  value: Value,
  callback: (error?: string | Error) => void
) => {
  const { old_password } = formData

  if (!value) {
    return callback(new Error('请再次确认旧密码'))
  }

  if (value !== old_password) {
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
  const { old_password, old_password2 } = formData

  if (!value) {
    return callback(new Error('请输入新密码'))
  }

  if (old_password && old_password2 && old_password === old_password2) {
    if (value === old_password) {
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
  old_password: [
    { required: true, message: '请填写旧密码', trigger: 'blur' },
    { min: 6, max: 20, message: '旧密码长度不合法', trigger: 'blur' }
  ],
  old_password2: [
    { required: true, message: '请填写旧密码', trigger: 'blur' },
    { min: 6, max: 20, message: '旧密码长度不合法', trigger: 'blur' },
    { validator: validateOldPwdEqual, trigger: 'blur' }
  ],
  new_password: [
    { required: true, message: '请填写新密码', trigger: 'blur' },
    { min: 6, max: 20, message: '新密码长度不合法', trigger: 'blur' },
    { validator: validateNewPwdNotEqualOld, trigger: 'blur' }
  ]
})

/**
 * 检查数据是否发生变化，且满足长度规则
 */
const checkUpdatable = (): boolean => {
  const old_passwordLength = formData.old_password.length
  const new_passwordLength = formData.new_password.length

  return (
    old_passwordLength >= 6 &&
    old_passwordLength <= 20 &&
    new_passwordLength >= 6 &&
    new_passwordLength <= 20 &&
    formData.old_password === formData.old_password2 &&
    formData.new_password !== formData.old_password
  )
}

/**
 * 更新密码按钮，校验表单数据并提交请求
 */
const changePasswordBtn = async (formEl?: FormInstance) => {
  const isValid = formEl?.validate()
  if (isValid) {
    const request: UserPasswordUpdateRequest = {
      old_password: formData.old_password,
      new_password: formData.new_password
    }

    await changePassword(request)
    ElMessage.success('密码修改成功！')
    formData.old_password = ''
    formData.old_password2 = ''
    formData.new_password = ''
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
    <el-form-item label="旧密码" prop="old_password">
      <el-input
        type="password"
        v-model="formData.old_password"
        placeholder="请输入旧密码"
      />
    </el-form-item>
    <el-form-item label="旧密码（确认）" prop="old_password2">
      <el-input
        type="password"
        v-model="formData.old_password2"
        placeholder="请再次输入旧密码"
      />
    </el-form-item>
    <el-form-item label="新密码" prop="new_password">
      <el-input
        type="password"
        v-model="formData.new_password"
        placeholder="请输入新密码"
      />
    </el-form-item>
  </el-form>
  <div class="update-btn">
    <el-button :disabled="!checkUpdatable()" @click="changePasswordBtn(formRef)"
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
