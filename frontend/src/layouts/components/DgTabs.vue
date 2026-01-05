<script setup lang="ts">
import { computed } from 'vue'
import { useRouter } from 'vue-router'
import { useCommonStore } from '@/stores/common'
import type { PageItem } from '@/types/common'

const router = useRouter()

const commonStore = useCommonStore()
const pageList = computed(() => commonStore.pageList)
const activePage = computed(
  () => commonStore.activePath[commonStore.activePath.length - 1].path
)

/**
 * 关闭一个页面
 * @param page
 */
function closePage(page: PageItem) {
  const newIndex = commonStore.pageListRemoveByPath(page.path)
  // 返回为非-1，表明关闭的是正在浏览的页面，需要跳转
  if (newIndex !== -1) {
    const newPath = pageList.value[newIndex].path
    router.push(newPath)
  }
}
</script>

<template>
  <ul class="tabs">
    <li
      class="tab"
      v-for="page in pageList"
      :class="{ active: page.path === activePage }"
      :style="{
        'padding-right':
          pageList.length > 1 && page.path !== '/home' ? '5px' : '10px'
      }"
      @click="router.push(page.path)"
    >
      {{ page.name }}
      <el-icon
        class="close"
        v-if="pageList.length > 1 && page.path !== '/home'"
        @click.stop="closePage(page)"
      >
        <Close />
      </el-icon>
    </li>
  </ul>
</template>

<style scoped lang="scss">
.tabs {
  list-style-type: none;
  display: flex;
  margin: 0;
  padding-left: 0;
  .tab {
    display: inline-block;
    background-color: #fff;
    border: 1px solid rgb(233, 233, 233);
    padding: 0 {
      left: 10px;
    }
    cursor: pointer;
    height: 32px;
    line-height: 32px;
    border-radius: 5px;
    &:not(:first-child) {
      margin-left: 10px;
    }
    &:hover,
    &.active {
      color: #409eff;
    }
    &.active {
      border-color: #409eff;
    }
    .close {
      margin-left: 3px;
      position: relative;
      top: 0.5px;
      width: 18px;
      height: 18px;
      border-radius: 50%;
      &:hover {
        background-color: #eee;
      }
      svg {
        width: 10px;
        height: 10px;
      }
    }
  }
}
</style>
