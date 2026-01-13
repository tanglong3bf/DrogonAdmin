<script lang="ts" setup>
import { useAuthStore } from '@/stores/auth'
import { UserInfo } from '@/types/auth'
import { Icon } from '@iconify/vue'
import { computed } from 'vue'
import { useRouter } from 'vue-router/dist/vue-router.mjs'

const authStore = useAuthStore()
const router = useRouter()

/**
 * 退出登录
 */
const logout = async () => {
  // 先跳页面后清数据，不会报错
  await router.push('/login')
  authStore.setUserInfo(undefined)
  authStore.setToken(undefined)
}

const userInfo = computed<UserInfo>(() => authStore.userInfo!)

const toUserCenter = () => router.push('/user-center')
</script>

<template>
  <el-dropdown placement="bottom-end">
    <el-avatar class="avatar" :size="36" :src="userInfo.avatar" />
    <template #dropdown>
      <div class="wrapper">
        <div class="header">
          <el-avatar :size="40" :src="userInfo.avatar" />
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
