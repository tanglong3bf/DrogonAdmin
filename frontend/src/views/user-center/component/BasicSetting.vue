<script lang="ts" setup>
import { watch, ref } from 'vue'
import { useAuthStore } from '@/stores/auth'
import { ElMessage, FormInstance, FormRules } from 'element-plus/es'
import { onMounted, reactive } from 'vue'
import { updateUserBasicInfo } from '@/api/user_center'
import { UserInfoUpdateRequest } from '@/types/user'
import { Sex } from '@/types/enums'

const authStore = useAuthStore()

/**
 * 可以更新的用户信息
 */
interface UserUpdatableInfo {
  nickname: string
  sex: Sex
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
  sex: 0,
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
 * 提交按钮可用
 */
const isBtnAvailable = ref(false)
watch(
  [() => formData, () => authStore.userInfo],
  async ([currentForm, userInfo]) => {
    if (!userInfo || !formRef.value) {
      isBtnAvailable.value = false
      return
    }

    // 字段是否发生修改
    const someFieldChanged =
      currentForm.nickname !== userInfo.nickname ||
      currentForm.sex !== userInfo.sex ||
      currentForm.phoneNumber !== userInfo.phone_number ||
      currentForm.email !== userInfo.email

    // 表单校验
    const allFieldAvailable = await formRef.value.validate()

    isBtnAvailable.value = someFieldChanged && allFieldAvailable
  },
  {
    deep: true,
    immediate: true
  }
)

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

    if (
      !nicknameChanged &&
      !sexChanged &&
      !phoneNumberChanged &&
      !emailChanged
    ) {
      ElMessage('所有信息未发生修改，无需提交')
      // 正常这按钮不会亮的
      console.warn('请检查isBtnAvailable的watch逻辑')
      return
    }

    // 梭哈
    const request: UserInfoUpdateRequest = {
      nickname: nicknameChanged ? formData.nickname : undefined,
      sex: sexChanged ? formData.sex : undefined,
      phone_number: phoneNumberChanged
        ? newPhoneNumberIsEmpty
          ? null
          : formData.phoneNumber
        : undefined,
      email: emailChanged ? (emailIsEmpty ? null : formData.email) : undefined
    }
    await updateUserBasicInfo(request)
    ElMessage.success('更新成功！')
    if (nicknameChanged) {
      userInfo.nickname = formData.nickname
    }
    if (sexChanged) {
      userInfo.sex = formData.sex
    }
    if (phoneNumberChanged) {
      userInfo.phone_number =
        formData.phoneNumber === '' ? undefined : formData.phoneNumber
    }
    if (emailChanged) {
      userInfo.email = formData.email === '' ? undefined : formData.email
    }
    authStore.setUserInfo(userInfo)
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
    <el-form-item label="昵称" prop="nickname">
      <el-input v-model="formData.nickname" placeholder="请输入你的昵称" />
    </el-form-item>
    <el-form-item label="性别" prop="sex">
      <el-select v-model="formData.sex" placeholder="请选择你的性别">
        <el-option label="男" :value="Sex.Male" />
        <el-option label="女" :value="Sex.Female" />
        <el-option label="保密" :value="Sex.Secrecy" />
      </el-select>
    </el-form-item>
    <el-form-item label="手机号码" prop="phone_number">
      <el-input
        v-model="formData.phoneNumber"
        placeholder="请输入你的手机号码"
      />
    </el-form-item>
    <el-form-item label="邮箱" prop="email">
      <el-input v-model="formData.email" placeholder="请输入你的邮箱" />
    </el-form-item>
    <div class="update-btn">
      <el-button :disabled="!isBtnAvailable" @click="updateBasicInfo(formRef)"
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
