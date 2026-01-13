<script lang="ts" setup>
import { computed, ref } from 'vue'
import { useAuthStore } from '@/stores/auth'
import { FormInstance, FormRules } from 'element-plus/es'
import { onMounted, reactive } from 'vue'
import { updateUserInfo } from '@/api/user'
import { UserInfoUpdateRequest } from '@/types/user'

const authStore = useAuthStore()

/**
 * 可以更新的用户信息
 */
interface UserUpdatableInfo {
  nickname: string
  sex: 'male' | 'female' | 'unknown'
  phoneNumber?: string
  email?: string
}

/**
 * 表单实体的引用
 */
const formRef = ref<FormInstance>()

/**
 * 表单数据
 */
const formData = reactive<UserUpdatableInfo>({
  nickname: '',
  sex: 'unknown',
  phoneNumber: undefined,
  email: undefined
})

/**
 * 初始化表单数据
 */
onMounted(() => {
  const userInfo = authStore.userInfo!

  formData.nickname = userInfo.nickname
  formData.sex = userInfo.sex
  formData.phoneNumber = userInfo.phone_number
  formData.email = userInfo.email
})

/**
 * 有字段发生了修改
 */
const someFieldChanged = computed(() => {
  const userInfo = authStore.userInfo!
  return (
    formData.email !== userInfo.email ||
    formData.nickname !== userInfo.nickname ||
    formData.sex !== userInfo.sex ||
    formData.phoneNumber !== userInfo.phone_number
  )
})

/**
 * 表单校验
 */
const rules = reactive<FormRules<UserUpdatableInfo>>({
  nickname: { required: true, message: '请填写昵称', trigger: 'blur' },
  sex: { required: true, message: '请选择性别', trigger: 'blur' },
  phoneNumber: {
    pattern:
      /^1(?:3[0-9]|4[013456789]|5[0-35-9]|6[0-9]|7[0-8]|8[0-9]|9[0-9])\d{8}$/,
    message: '手机号格式错误',
    trigger: 'blur'
  },
  email: {
    pattern: /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/,
    message: '邮箱格式错误',
    trigger: 'blur'
  }
})

/**
 * 按钮验证并提交表单
 */
const updateBasicInfo = async (formEl?: FormInstance) => {
  const isValid = await formEl?.validate()
  if (isValid) {
    const userInfo = authStore.userInfo!

    // 各种条件检查
    const nicknameChanged = formData.nickname !== userInfo.nickname
    const sexChanged = formData.sex !== userInfo.sex
    const phoneNumberChanged = formData.phoneNumber !== userInfo.phone_number
    const newPhoneNumberIsEmpty =
      formData.phoneNumber === undefined || formData.phoneNumber.length === 0
    const emailChanged = formData.email !== userInfo.email
    const emailIsEmpty =
      formData.email === undefined || formData.email.length === 0

    // 梭哈
    const request: UserInfoUpdateRequest = {
      user_id: userInfo.user_id,
      nickname: nicknameChanged ? formData.nickname : undefined,
      sex: sexChanged ? formData.sex : undefined,
      phone_number: phoneNumberChanged
        ? newPhoneNumberIsEmpty
          ? null
          : formData.phoneNumber
        : undefined,
      email: emailChanged ? (emailIsEmpty ? null : formData.email) : undefined
    }
    await updateUserInfo(request)
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
    <el-form-item label="昵称" prop="nickname" placeholder="请输入你的昵称">
      <el-input v-model="formData.nickname" />
    </el-form-item>
    <el-form-item label="性别" prop="sex">
      <el-select v-model="formData.sex" placeholder="请选择你的性别">
        <el-option label="男" value="male" />
        <el-option label="女" value="female" />
        <el-option label="保密" value="unknown" />
      </el-select>
    </el-form-item>
    <el-form-item
      label="手机号码"
      prop="phone_number"
      placeholder="请输入你的手机号码"
    >
      <el-input v-model="formData.phoneNumber" />
    </el-form-item>
    <el-form-item label="邮箱" prop="email" placeholder="请输入你的邮箱">
      <el-input v-model="formData.email" />
    </el-form-item>
    <div class="update-btn">
      <el-button :disabled="!someFieldChanged" @click="updateBasicInfo(formRef)"
        >更新</el-button
      >
    </div>
  </el-form>
</template>

<style lang="scss" scoped>
.update-btn {
  display: flex;
  flex-direction: row-reverse;
}
</style>
