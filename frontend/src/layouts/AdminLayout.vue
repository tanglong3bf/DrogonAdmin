<script lang="ts" setup>
import { onMounted, ref, watch } from 'vue'
import Header from './components/Header.vue'
import Aside from './components/Aside.vue'
import { RouteRecordRaw, useRouter } from 'vue-router/dist/vue-router.mjs'
import { useCommonStore } from '@/stores/common'
import { Menu } from '@/types/menu'

const menuExpand = ref(true)

const viewsComponents = import.meta.glob('/src/views/**/*.vue')

const router = useRouter()
const commonStore = useCommonStore()

/** 上一次注入的动态路由，用于增量对比 */
let injectedRoutes: RouteRecordRaw[] = []

// 生成路由配置
const generateRoutes = (menuList: Menu[]): RouteRecordRaw[] => {
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

/**
 * 按侧边栏菜单同步动态路由
 *
 * 菜单管理页增删改菜单后会更新 menuList，这里按差异移除/新增路由，
 * 未变化的菜单不会重复移除，避免影响当前正在展示的页面
 */
const syncRoutes = () => {
  // 过滤掉静态路由
  const routes = generateRoutes(
    commonStore.menuList.filter(
      item => item.path !== '/home' && item.path !== '/user-center'
    )
  )

  // 移除已被删除或已变化的菜单路由
  injectedRoutes.forEach(injected => {
    const stillExists = routes.some(
      route =>
        route.name === injected.name &&
        route.path === injected.path &&
        route.component === injected.component
    )
    if (!stillExists && router.hasRoute(injected.name!)) {
      router.removeRoute(injected.name!)
    }
  })

  // 注入新增的菜单路由
  routes.forEach(route => {
    if (router.hasRoute(route.name!) === false) {
      router.addRoute('root', route)
    }
  })

  injectedRoutes = routes
}

onMounted(syncRoutes)

watch(() => commonStore.menuList, syncRoutes, { deep: true })
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
