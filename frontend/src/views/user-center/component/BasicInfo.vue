<script lang="ts" setup>
import { computed, ref } from 'vue'
import { useAuthStore } from '@/stores/auth'
import type { UserInfo } from '@/types/auth'
import { ElMessage, ElUpload, UploadFile } from 'element-plus/es'
import { uploadAvatar } from '@/api/user_center'
import { Icon } from '@iconify/vue'
import { Sex } from '@/types/enums'
import { IMG_BASE_URL } from '@/config'

const authStore = useAuthStore()

const userInfo = computed<UserInfo>(() => authStore.userInfo!)

const uploadRef = ref<InstanceType<typeof ElUpload> | null>(null)
// 本地预览图base64
const avatarPreview = ref<string | null>(null)
// 待上传文件
const avatarFile = ref<File | null>(null)
const isUploading = ref<boolean>(false)

/**
 * 处理文件选择变化：实现本地预览
 * @param uploadFile - 上传文件对象
 */
const handleFileChange = (uploadFile: UploadFile) => {
  const file = uploadFile.raw // 获取原生File对象
  if (!file) return

  // 校验文件
  if (!beforeAvatarUpload(file)) {
    uploadRef.value?.clearFiles()
    return
  }

  // 通过FileReader将文件转换为base64，实现本地预览
  const reader = new FileReader()
  reader.onload = e => {
    avatarPreview.value = e.target?.result as string
  }
  reader.readAsDataURL(file)

  // 缓存文件实例，用于后续手动上传
  avatarFile.value = file
}

/**
 * 恢复头像
 */
const resetAvatar = () => {
  avatarFile.value = null
  avatarPreview.value = null
}

/**
 * 文件上传前校验（格式、大小）
 * @param file - 原生File对象
 */
const beforeAvatarUpload = (file: File) => {
  // 校验文件类型
  const isImage = file.type.startsWith('image/')
  if (!isImage) {
    ElMessage.error('只能上传图片格式文件（jpg、png、jpeg等）！')
    return false
  }

  // 校验文件大小
  const isLt2M = file.size / 1024 / 1024 < 2
  if (!isLt2M) {
    ElMessage.error('头像图片大小不能超过2MB！')
    return false
  }

  return true
}

/**
 * 自定义手动上传逻辑
 * @param params - el-upload传入的上传参数
 */
const handleManualUpload = async (_params: any) => {
  if (!avatarFile.value) return
  isUploading.value = true
  try {
    const response = await uploadAvatar(avatarFile.value)
    const { file_path: filePath } = response
    authStore.setUserInfo({
      ...userInfo.value,
      avatar: filePath
    })
  } catch (error) {
    console.error('上传请求异常：', error)
    ElMessage.error('头像上传失败，请稍后重试！')
  } finally {
    isUploading.value = false
    uploadRef.value?.clearFiles()
    resetAvatar()
  }
}

/**
 * 点击确认上传按钮：触发手动上传
 */
const handleConfirmUpload = () => {
  if (!avatarFile.value) {
    console.warn('按理说不会走到这里的，去查代码')
    ElMessage.warning('请先选择要上传的头像图片！')
    return
  }
  uploadRef.value?.submit()
}

/**
 * 头像预览
 */
const avatarPreviewUrl = computed(() => {
  if (avatarPreview.value !== null) {
    return avatarPreview.value
  }
  if (
    userInfo.value.avatar.startsWith('http://') ||
    userInfo.value.avatar.startsWith('https://')
  ) {
    return userInfo.value.avatar
  }
  if (userInfo.value.avatar === '#') {
    switch (userInfo.value.sex) {
      case Sex.Secrecy:
        return new URL(`@/assets/avatar/drogon-logo.svg`, import.meta.url).href
      case Sex.Male:
        return new URL(`@/assets/avatar/male.jpeg`, import.meta.url).href
      case Sex.Female:
        return new URL(`@/assets/avatar/female.jpeg`, import.meta.url).href
    }
  }
  return IMG_BASE_URL + userInfo.value.avatar
})

const email = computed(() => {
  return userInfo.value.email === undefined ? '(未设置)' : userInfo.value.email
})
const phoneNumber = computed(() => {
  return userInfo.value.phone_number === undefined
    ? '(未设置)'
    : userInfo.value.phone_number
})
</script>

<template>
  <el-card style="border-radius: 12px">
    <template #header>
      <span>个人信息</span>
    </template>
    <!-- 头像 -->
    <div class="avatar">
      <div class="block">
        <div class="avatar-upload-container">
          <!-- 头像上传组件：关闭自动上传，自定义上传逻辑 -->
          <el-upload
            class="avatar-uploader"
            :auto-upload="false"
            :show-file-list="false"
            :on-change="handleFileChange"
            :before-upload="beforeAvatarUpload"
            :http-request="handleManualUpload"
            ref="uploadRef"
          >
            <img :src="avatarPreviewUrl" class="avatar" alt="头像预览" />
          </el-upload>
          <br />
        </div>
      </div>
      <div class="avatar-btn">
        <el-button v-if="avatarFile" @click="resetAvatar">恢复头像</el-button>
        <el-button
          type="primary"
          class="confirm-upload-btn"
          @click="handleConfirmUpload"
          :disabled="!avatarFile"
          :loading="isUploading"
        >
          更新头像
        </el-button>
      </div>
    </div>
    <el-divider />
    <!-- 展示基础信息 -->
    <ul class="user-info">
      <li class="user-info-item">
        <div class="text">
          <el-icon><Icon icon="ep:user" /></el-icon>
          <span class="value">{{ userInfo.username }}</span>
        </div>
      </li>
      <li class="user-info-item">
        <div class="text">
          <el-icon><Icon icon="ep:user-filled" /></el-icon>
          <span class="value">{{ userInfo.nickname }}</span>
        </div>
      </li>
      <li class="user-info-item">
        <div class="text">
          <el-icon><Icon icon="mdi:gender-male-female" /></el-icon>
          <span class="value">{{
            userInfo.sex === Sex.Male
              ? '男'
              : userInfo.sex === Sex.Female
                ? '女'
                : '保密'
          }}</span>
        </div>
      </li>
      <li class="user-info-item">
        <div class="text">
          <el-icon><Icon icon="ep:phone" /></el-icon>
          <span class="value">{{ phoneNumber }}</span>
        </div>
      </li>
      <li class="user-info-item">
        <div class="text">
          <el-icon><Icon icon="ep:message" /></el-icon>
          <span class="value">{{ email }}</span>
        </div>
      </li>
    </ul>
  </el-card>
</template>

<style lang="scss" scoped>
.avatar {
  width: 100%;
  .block {
    margin: 20px auto;
    width: 188px;
  }
  .avatar {
    width: 188px;
    height: 188px;
  }
  .avatar-btn {
    display: flex;
    justify-content: center;
  }
}
.user-info {
  .user-info-item {
    display: flex;
    margin-top: 15px;
    margin-left: 25px;
    .value {
      color: gray;
      margin-left: 15px;
    }
  }
}
</style>
