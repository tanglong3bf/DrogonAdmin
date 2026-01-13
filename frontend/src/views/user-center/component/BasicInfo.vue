<script lang="ts" setup>
import { computed } from 'vue'
import { useAuthStore } from '@/stores/auth'
import type { UserInfo } from '@/types/auth'

const authStore = useAuthStore()

const userInfo = computed<UserInfo>(() => authStore.userInfo!)
</script>

<template>
  <el-card style="border-radius: 12px">
    <template #header>
      <span>个人信息</span>
    </template>
    <!-- 头像 -->
    <div class="avatar">
      <div class="block">
        <el-avatar
          tag="div"
          shape="square"
          :size="150"
          :src="userInfo.avatar"
        />
      </div>
      <div class="avatar-btn">
        <!-- TODO: -->
        <el-button>更换头像</el-button>
      </div>
    </div>
    <el-divider />
    <!-- 展示基础信息 -->
    <!-- TODO: 缺图标-->
    <ul class="user-info">
      <li class="user-info-item">
        <el-icon></el-icon>
        <div class="text">
          用户名：<span class="value">{{ userInfo.username }}</span>
        </div>
      </li>
      <li class="user-info-item">
        <el-icon></el-icon>
        <div class="text">
          昵称：<span class="value">{{ userInfo.nickname }}</span>
        </div>
      </li>
      <li class="user-info-item">
        <el-icon></el-icon>
        <div class="text">
          性别：<span class="value">{{ userInfo.sex }}</span>
        </div>
      </li>
      <li class="user-info-item">
        <el-icon></el-icon>
        <div class="text">
          邮箱：<span class="value">{{ userInfo.email }}</span>
        </div>
      </li>
      <li class="user-info-item">
        <el-icon></el-icon>
        <div class="text">
          手机号：<span class="value">{{ userInfo.phone_number }}</span>
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
    width: 150px;
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
    .value {
      color: gray;
    }
  }
}
</style>
