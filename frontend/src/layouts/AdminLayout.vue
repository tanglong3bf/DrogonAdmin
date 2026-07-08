<script lang="ts" setup>
import { onMounted, ref } from 'vue'
import Header from './components/Header.vue'
import Aside from './components/Aside.vue'
import { RouteRecordRaw, useRouter } from 'vue-router/dist/vue-router.mjs'
import { useCommonStore } from '@/stores/common'
import { SidebarMenu } from '@/types/menu'

const menuExpand = ref(true)

const viewsComponents = import.meta.glob('/src/views/**/*.vue')

onMounted(() => {
  const router = useRouter()
  const commonStore = useCommonStore()

  // 生成路由配置
  const generateRoutes = (menuList: SidebarMenu[]): RouteRecordRaw[] => {
    return menuList.flatMap(menu => {
      let currentRoute: RouteRecordRaw[] = []
      if (menu.path && menu.component) {
        const componentPath = `/src/views${menu.component}.vue`
        const component = viewsComponents[componentPath]
        currentRoute = [
          {
            path: menu.path,
            name: menu.name,
            component
          }
        ]
      }
      const childRoutes = menu.children ? generateRoutes(menu.children) : []
      return [...currentRoute, ...childRoutes]
    })
  }
  // 过滤掉静态路由
  const routes = generateRoutes(
    commonStore.menuList.filter(
      item => item.path !== '/home' && item.path !== '/user-center'
    )
  )

  // 避免重复添加
  routes
    .filter(route => router.hasRoute(route.name!) === false)
    .forEach(route => router.addRoute('root', route))
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
