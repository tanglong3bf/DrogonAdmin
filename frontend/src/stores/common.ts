import { ref } from 'vue'
import { defineStore } from 'pinia'
import { PageItem } from '@/types/common'
import { SidebarMenu } from '@/types/menu'

export const useCommonStore = defineStore(
  'common',
  () => {
    const menuList = ref<SidebarMenu[]>([])

    /**
     * 设置侧边栏菜单
     * @param newMenuList
     */
    const setMenuList = (newMenuList: SidebarMenu[]) => {
      menuList.value = [
        {
          path: '/home',
          index: '0',
          icon: 'ion:home-sharp',
          name: '首页',
          component: '/Home'
        },
        ...newMenuList
      ]
    }

    /**
     * 页面标签的列表
     */
    const pageList = ref<PageItem[]>([
      {
        name: '首页',
        path: '/home'
      }
    ])

    /**
     * 页面标签添加新项
     * @param newPage
     */
    const pageListAddItem = (newPage: PageItem) => {
      const newPageList = pageList.value.find(
        page => page.path === newPage.path
      )
        ? pageList.value
        : [...pageList.value, newPage]

      pageList.value = newPageList
    }

    /**
     * 删除指定项
     * @param path
     * @returns activePath在pageList里的下标，-1表示未发生变化
     */
    const pageListRemoveByPath = (path: string) => {
      const index = pageList.value.findIndex(page => page.path === path)
      if (index === -1) return -1

      const isActive =
        activePath.value[activePath.value.length - 1].path === path
      const newPageList = pageList.value.filter(page => page.path !== path)
      const newIndex = isActive
        ? index === pageList.value.length - 1
          ? index - 1
          : index
        : -1

      setActivePath(isActive ? [newPageList[newIndex]] : activePath.value)
      pageList.value = newPageList
      return newIndex
    }

    /**
     * 当前展示页面，包含父级，用于展示面包屑
     */
    const activePath = ref<PageItem[]>([])

    /**
     * 设置当前展示页面
     * @param newActivePath
     */
    const setActivePath = (newActivePath: PageItem[]) => {
      const page = newActivePath[newActivePath.length - 1]
      if (page.path !== '/login') {
        const pageToAdd = page.path === '/' ? { ...page, path: '/home' } : page
        pageListAddItem(pageToAdd)
      }
      activePath.value = newActivePath
    }

    return {
      menuList,
      setMenuList,
      pageList,
      pageListRemoveByPath,
      activePath,
      setActivePath
    }
  },
  {
    persist: true
  }
)
