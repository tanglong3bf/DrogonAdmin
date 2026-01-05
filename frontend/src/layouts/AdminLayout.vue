<script lang="ts" setup>
import { ref } from 'vue'
import Header from './components/Header.vue'
import Aside from './components/Aside.vue'

const menuExpand = ref(true)
</script>

<template>
  <el-container class="layout-container">
    <el-aside :width="menuExpand ? '220px' : '80px'">
      <Aside :menuExpand="menuExpand" />
    </el-aside>
    <el-container>
      <el-header height="104px">
        <Header
          :menuExpand="menuExpand"
          @toggleMenu="menuExpand = !menuExpand"
        />
      </el-header>
      <el-main>
        <router-view />
      </el-main>
    </el-container>
  </el-container>
</template>

<style lang="scss" scoped>
.layout-container {
  width: 100%;
  height: 100%;
  .el-aside {
    border-right: 1px solid #dbdfe999;
    transition-duration: 0.3s;
  }
  .el-main {
    padding-top: 0;
  }
  .el-header,
  .el-main {
    background-color: #fafafa;
  }
}
</style>
