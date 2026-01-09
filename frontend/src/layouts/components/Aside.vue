<script setup lang="ts">
import { useRoute, useRouter } from 'vue-router/dist/vue-router.mjs'
import { useCommonStore } from '@/stores/common'
import MenuItem from './MenuItem.vue'

const router = useRouter()
const route = useRoute()
const commonStore = useCommonStore()

defineProps<{ menuExpand: boolean }>()

/**
 * 选中某一个菜单
 */
const menuSelect = (to: string) => {
  const isOutLink = to.startsWith('http://') || to.startsWith('https://')

  // 外链直接跳转
  isOutLink ? window.open(to, '_blank') : router.push(to)
}
</script>

<template>
  <div class="header">
    <el-image class="logo" src="/src/assets/drogon-logo.svg" fit="fill" />
    <el-text v-if="menuExpand" class="website-name" size="large" line-clamp="1"
      >Drogon Admin</el-text
    >
  </div>
  <el-scrollbar>
    <el-menu
      :default-active="route.path"
      unique-opened
      @select="menuSelect"
      :collapse="!menuExpand"
    >
      <MenuItem :list="commonStore.menuList" />
    </el-menu>
  </el-scrollbar>
</template>

<style lang="scss" scoped>
:deep(.el-menu--collapse) {
  width: auto !important;
}
.header {
  height: 60px;
  display: flex;
  justify-content: flex-start;
  align-items: center;
  .logo {
    width: 36px !important;
    height: 36px;
    margin-left: 28px;
    flex-shrink: 0;
  }
  .website-name {
    margin-left: 5px;
    opacity: 1;
    font-weight: 500;
  }
}
.el-scrollbar {
  height: calc(100% - 60px);
  .el-menu {
    border-right: 0;
  }
}
</style>
