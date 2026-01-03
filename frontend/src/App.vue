<script setup lang="ts">
import zhCN from 'element-plus/es/locale/lang/zh-cn'
import { ElConfigProvider } from 'element-plus'
</script>

<template>
  <el-config-provider :locale="zhCN">
    <RouterView />
  </el-config-provider>
</template>

<style scoped lang="scss"></style>
