<script setup lang="ts">
import { computed, ref, watch } from 'vue'
import { Icon } from '@iconify/vue'
import { iconCollections } from '@/plugins/iconify'

const props = defineProps<{
  /** 当前选中的图标名，格式为 前缀:名称，如 ion:settings-sharp */
  modelValue?: string
}>()

const emit = defineEmits<{
  (e: 'update:modelValue', value: string): void
}>()

/** 每页渲染的图标数量，避免一次性渲染上万个节点 */
const PAGE_SIZE = 100

const visible = ref(false)
/** 搜索关键词，仅用于过滤，不代表选中值 */
const keyword = ref('')
const activePrefix = ref('all')
const currentPage = ref(1)
/** 弹窗内真实选中的图标，点击确定后才回传给父组件 */
const draft = ref('')

/** 所有可用的图标集合前缀 */
const prefixes = computed(() =>
  iconCollections.map(collection => collection.prefix)
)

/** 当前筛选条件下命中的全部图标名 */
const matchedIcons = computed(() => {
  const result: string[] = []
  for (const collection of iconCollections) {
    if (
      activePrefix.value !== 'all' &&
      collection.prefix !== activePrefix.value
    ) {
      continue
    }
    for (const name of Object.keys(collection.icons)) {
      result.push(`${collection.prefix}:${name}`)
    }
    for (const name of Object.keys(collection.aliases ?? {})) {
      result.push(`${collection.prefix}:${name}`)
    }
  }

  const text = keyword.value.trim().toLowerCase()
  if (!text) return result
  return result.filter(name => name.toLowerCase().includes(text))
})

/** 当前页实际渲染的图标 */
const visibleIcons = computed(() => {
  const start = (currentPage.value - 1) * PAGE_SIZE
  return matchedIcons.value.slice(start, start + PAGE_SIZE)
})

watch(
  [keyword, activePrefix],
  () => {
    currentPage.value = 1
  },
  { flush: 'sync' }
)

const open = () => {
  keyword.value = ''
  const value = props.modelValue?.trim() ?? ''
  const separatorIndex = value.indexOf(':')
  const prefix = separatorIndex > 0 ? value.slice(0, separatorIndex) : ''
  activePrefix.value = prefixes.value.includes(prefix) ? prefix : 'all'
  draft.value = value

  // 有旧值时直接定位到该图标所在的页
  const index = matchedIcons.value.indexOf(draft.value)
  currentPage.value = index >= 0 ? Math.floor(index / PAGE_SIZE) + 1 : 1
  visible.value = true
}

const pick = (name: string) => {
  draft.value = name
}

const clear = () => {
  draft.value = ''
}

const confirm = () => {
  emit('update:modelValue', draft.value)
  visible.value = false
}
</script>

<template>
  <el-input
    :model-value="props.modelValue"
    readonly
    placeholder="点击选择图标"
    class="icon-picker-trigger"
    @click="open"
  >
    <template #prepend>
      <Icon v-if="props.modelValue" :icon="props.modelValue" width="18" />
      <span v-else>图标</span>
    </template>
  </el-input>

  <el-dialog v-model="visible" title="选择图标" width="760px" append-to-body>
    <div class="icon-picker-toolbar">
      <el-input
        v-model="keyword"
        placeholder="搜索图标名称，如 user、home"
        clearable
      />
      <el-select v-model="activePrefix" class="icon-picker-prefix">
        <el-option value="all" label="全部集合" />
        <el-option
          v-for="prefix in prefixes"
          :key="prefix"
          :value="prefix"
          :label="prefix"
        />
      </el-select>
    </div>

    <!-- 真实选中的图标值，不直接展示 -->
    <input type="hidden" :value="draft" />

    <div v-if="visibleIcons.length" class="icon-picker-grid">
      <button
        v-for="name in visibleIcons"
        :key="name"
        type="button"
        class="icon-picker-cell"
        :class="{ 'is-active': name === draft }"
        :title="name"
        @click="pick(name)"
      >
        <Icon :icon="name" width="22" height="22" />
      </button>
    </div>
    <el-empty v-else description="没有匹配的图标" :image-size="80" />

    <div class="icon-picker-pager">
      <span class="icon-picker-count">共 {{ matchedIcons.length }} 个图标</span>
      <el-pagination
        v-model:current-page="currentPage"
        :page-size="PAGE_SIZE"
        :total="matchedIcons.length"
        layout="prev, pager, next"
        background
        small
      />
    </div>

    <template #footer>
      <div class="icon-picker-footer">
        <el-button size="small" @click="clear">清空</el-button>
        <div>
          <el-button size="small" @click="visible = false">取消</el-button>
          <el-button size="small" type="primary" @click="confirm">
            确定
          </el-button>
        </div>
      </div>
    </template>
  </el-dialog>
</template>

<style lang="scss" scoped>
.icon-picker-trigger {
  cursor: pointer;

  :deep(.el-input__wrapper) {
    cursor: pointer;
  }

  :deep(.el-input__inner) {
    cursor: pointer;
  }
}

.icon-picker-toolbar {
  display: flex;
  gap: 12px;
  margin-bottom: 12px;
}

.icon-picker-prefix {
  width: 150px;
  flex-shrink: 0;
}

.icon-picker-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(44px, 1fr));
  gap: 6px;
  max-height: 340px;
  overflow-y: auto;
  padding: 4px;
}

.icon-picker-cell {
  display: flex;
  align-items: center;
  justify-content: center;
  height: 44px;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: 4px;
  background-color: var(--el-fill-color-blank);
  color: var(--el-text-color-regular);
  cursor: pointer;
  transition: all 0.15s;

  &:hover {
    border-color: var(--el-color-primary);
    color: var(--el-color-primary);
  }

  &.is-active {
    border-color: var(--el-color-primary);
    color: var(--el-color-primary);
    background-color: var(--el-color-primary-light-9);
  }
}

.icon-picker-pager {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  flex-wrap: wrap;
  margin-top: 12px;
}

.icon-picker-count {
  font-size: 12px;
  color: var(--el-text-color-secondary);
}

.icon-picker-footer {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
}
</style>
