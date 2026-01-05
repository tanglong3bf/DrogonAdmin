<script setup lang="ts">
import { ref } from 'vue'
import { useCommonStore } from '@/stores/common'
import DgTabs from './DgTabs.vue'

const commonStore = useCommonStore()

defineProps<{ menuExpand: boolean }>()
const emit = defineEmits(['toggleMenu'])

/**
 * 解决横向滚动的问题
 */
const scrollbarRef = ref()
function handleScroll(e: any) {
  const wheelDelta = e.wheelDelta || -e.deltaY * 40

  scrollbarRef.value.setScrollLeft(
    scrollbarRef.value.wrapRef.scrollLeft - wheelDelta
  )
}
</script>

<template>
  <div id="header">
    <div id="left">
      <div class="btn-box" @click="$emit('toggleMenu')">
        <div class="btn menu">
          <el-icon v-if="!menuExpand"><Expand /></el-icon>
          <el-icon v-else><Fold /></el-icon>
        </div>
      </div>
      <el-breadcrumb separator="/">
        <el-breadcrumb-item
          v-for="item in commonStore.activePath"
          :to="{ path: item.path }"
        >
          {{ item.name }}
        </el-breadcrumb-item>
      </el-breadcrumb>
    </div>
    <div id="right"></div>
  </div>

  <div id="worktab">
    <el-scrollbar ref="scrollbarRef" @wheel.passive.pevent="handleScroll">
      <DgTabs />
    </el-scrollbar>
  </div>
</template>

<style scoped lang="scss">
#header {
  height: 60px;
  display: flex;
  margin-left: -20px;
  justify-content: space-between;
  #left {
    display: flex;
    align-items: center;
  }
}

.btn-box {
  width: 60px;
  height: 60px;
  cursor: pointer;
  & > .btn {
    margin: 10px;
    width: 40px;
    height: 40px;
    border-radius: 8px;
  }
  &:hover > .btn {
    background-color: rgba(241, 241, 241, 0.7);
  }
  .el-icon {
    display: block;
    padding: 10px;
    svg {
      width: 20px;
      height: 20px;
      color: #78829d;
    }
  }
}

#worktab {
  height: 34px;
  white-space: nowrap;
}
:deep(.el-scrollbar__bar) {
  display: none !important;
}
:deep(.el-scrollbar__wrap) {
  overflow-x: hidden;
}
</style>
