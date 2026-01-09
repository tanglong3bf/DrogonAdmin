<script lang="ts" setup>
import { onMounted, ref } from 'vue'
import Header from './components/Header.vue'
import Aside from './components/Aside.vue'
import { RouteRecordRaw, useRouter } from 'vue-router/dist/vue-router.mjs'
import { useCommonStore } from '@/stores/common'
import { SidebarMenu } from '@/types/menu'

const menuExpand = ref(true)

// TODO: 刷新后控制台依然会报错，感觉很不优雅，想改，不知道咋改
onMounted(() => {
  const router = useRouter()
  const commonStore = useCommonStore()

  // 生成路由配置
  const generateRoutes = (menuList: SidebarMenu[]): RouteRecordRaw[] => {
    return menuList.flatMap(menu => {
      const currentRoute: RouteRecordRaw[] =
        menu.path && menu.component
          ? [
              {
                path: menu.path,
                name: menu.name,
                component: () =>
                  import(/* @vite-ignore */ `/src/views${menu.component}.vue`)
              }
            ]
          : []
      const childRoutes = menu.children ? generateRoutes(menu.children) : []
      return [...currentRoute, ...childRoutes]
    })
  }
  const routes = generateRoutes(
    commonStore.menuList.filter(item => item.path !== '/home')
  )

  routes.forEach(route => router.addRoute('root', route))
})
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
