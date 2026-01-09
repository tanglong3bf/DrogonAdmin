<script setup lang="ts">
import { Icon } from '@iconify/vue'
import type { SidebarMenu } from '@/types/menu'

defineProps<{ list: SidebarMenu[] }>()
</script>

<template>
  <template v-for="item in list">
    <el-menu-item
      v-if="!item.children || item.children.length === 0"
      :index="item.path"
    >
      <el-icon v-if="item.icon"><Icon :icon="item.icon" /></el-icon>
      <span :style="{ marginLeft: item.icon ? 0 : '29px' }">{{
        item.name
      }}</span>
    </el-menu-item>
    <el-sub-menu v-else :index="item.index">
      <template #title>
        <el-icon v-if="item.icon"><Icon :icon="item.icon" /></el-icon>
        <span :style="{ marginLeft: item.icon ? 0 : '29px' }">{{
          item.name
        }}</span>
      </template>
      <MenuItem :list="item.children" />
    </el-sub-menu>
  </template>
</template>

<style lang="scss" scoped>
.el-menu-item.is-active:not(.is-opened) {
  background-color: #ecf5ff;
}
:deep(.el-sub-menu__title:hover),
.el-menu-item:not(.is-active):hover {
  background-color: #f1f1f1;
}
:deep(.el-sub-menu__title),
.el-menu-item {
  border-radius: 6px;
  margin: 0 8px 4px 8px;
}
</style>

<style lang="scss">
.is-active > .el-sub-menu__title {
  color: #409eff;
}
</style>
