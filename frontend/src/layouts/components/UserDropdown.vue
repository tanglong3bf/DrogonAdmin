<script lang="ts" setup>
import { useAuthStore } from '@/stores/auth'
import { useCommonStore } from '@/stores/common'
import { UserInfo } from '@/types/auth'
import { Sex } from '@/types/enums'
import { Icon } from '@iconify/vue'
import { computed } from 'vue'
import { useRouter } from 'vue-router/dist/vue-router.mjs'
import { joinImageUrl } from '@/utils/url'

const authStore = useAuthStore()
const commonStore = useCommonStore()
const router = useRouter()

/**
 * 退出登录
 */
const logout = async () => {
  // 先跳页面后清数据，不会报错
  await router.push('/login')
  authStore.setUserInfo(undefined)
  authStore.setToken(undefined)
  commonStore.setMenuList([])
  commonStore.pageListClear()
}

const userInfo = computed<UserInfo>(() => authStore.userInfo!)

const toUserCenter = () => router.push('/user-center')

/**
 * 头像预览
 */
const avatarPreviewUrl = computed(() => {
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
  return joinImageUrl(userInfo.value.avatar)
})
</script>

<template>
  <el-dropdown placement="bottom-end">
    <el-avatar class="avatar" :size="36" :src="avatarPreviewUrl" />
    <template #dropdown>
      <div class="wrapper">
        <div class="header">
          <el-avatar :size="40" :src="avatarPreviewUrl" />
          <div style="display: flex; flex-direction: column">
            <span class="nickname">{{ userInfo.nickname }}</span>
            <span class="email" v-if="userInfo.email">{{
              userInfo.email
            }}</span>
          </div>
        </div>
        <el-dropdown-item divided class="btn-item" @click="toUserCenter"
          ><el-icon size="24"><Icon icon="prime:user" /></el-icon
          >个人中心</el-dropdown-item
        >
        <el-divider />
        <div class="logout-wrapper">
          <el-button class="logout-btn" @click="logout">退出登录</el-button>
        </div>
      </div>
    </template>
  </el-dropdown>
</template>

<style lang="scss" scoped>
.avatar {
  margin-top: 12px;
}
.wrapper {
  width: 240px;
  padding: 5px 16px;
  .header {
    display: flex;
    margin: 15px 0;
    .nickname {
      margin-left: 12px;
      font-size: 14px;
    }
    .email {
      margin-left: 12px;
      margin-top: 2px;
      font-size: 12px;
      color: #949eb7;
    }
  }
  :deep(.btn-item) {
    margin-top: 20px;
    margin-bottom: 20px;
  }
}
.logout-wrapper {
  width: 100%;
  display: flex;
  .logout-btn {
    margin: 0px auto 15px;
  }
}
</style>
