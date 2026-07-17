<script setup lang="ts">
import { Action } from '@/types/module'
import { ElMessage } from 'element-plus/es'
import { computed, ref, onUnmounted } from 'vue'

const props = defineProps<{
  actions: Action[]
  modelValue: { high: number; low: number }[]
}>()
const emit = defineEmits(['update:modelValue'])
const linkList = computed({
  get() {
    return props.modelValue
  },
  set(val) {
    emit('update:modelValue', val)
  }
})

// SVG 容器引用，用于坐标转换
const svgRef = ref<SVGSVGElement>()
// 拖拽状态
const dragging = ref(false)
const startDotId = ref<number | null>(null)
const tempEndPos = ref<{ x: number; y: number } | null>(null)

/**
 * 计算画布高度
 */
const height = computed(() => props.actions.length * 50 - 18)

/**
 * 生成所有节点坐标（固定 DOM 顺序，保证位移动画正常）
 */
const dots = computed(() => {
  const indexMap = new Map(props.actions.map((item, i) => [item.action_id, i]))
  // 按 action_id 固定排序
  return [...props.actions]
    .sort((a, b) => a.action_id - b.action_id)
    .map(item => ({
      id: item.action_id,
      x: 10,
      y: indexMap.get(item.action_id)! * 50 + 15
    }))
})

/**
 * 正式连线路径
 */
const linkPaths = computed(() => {
  const dotList = dots.value
  return linkList.value.map(link => {
    const fromDot = dotList.find(d => d.id === link.high)
    const toDot = dotList.find(d => d.id === link.low)

    if (!fromDot) throw new Error(`权限连线异常：不存在起点ID ${link.high}`)
    if (!toDot) throw new Error(`权限连线异常：不存在终点ID ${link.low}`)

    const { x: x1, y: y1 } = fromDot
    const { x: x2, y: y2 } = toDot
    const controlX = x1 + (y2 - y1) / 2.5 - 20
    const pathD = `M ${x1} ${y1} C ${controlX} ${y1}, ${controlX} ${y2}, ${x2} ${y2}`

    return {
      key: `${link.high}-${link.low}`,
      d: pathD
    }
  })
})

/**
 * 拖拽时的临时连线路径
 */
const tempLinkPath = computed(() => {
  if (!dragging.value || !tempEndPos.value || startDotId.value === null)
    return ''
  const startDot = dots.value.find(d => d.id === startDotId.value)
  if (!startDot) return ''

  const { x: x1, y: y1 } = startDot
  const { x: x2, y: y2 } = tempEndPos.value
  const controlX = x1 + Math.abs(y2 - y1) / 2.5 - 20
  return `M ${x1} ${y1} C ${controlX} ${y1}, ${controlX} ${y2}, ${x2} ${y2}`
})

/**
 * 按下节点：开始拖拽
 */
const handleMouseDown = (e: MouseEvent, dotId: number) => {
  e.preventDefault()
  e.stopPropagation()

  startDotId.value = dotId
  dragging.value = true

  // 初始化临时终点为起点位置
  const startDot = dots.value.find(d => d.id === dotId)
  if (startDot) {
    tempEndPos.value = { x: startDot.x, y: startDot.y }
  }

  document.addEventListener('mousemove', handleMouseMove)
  document.addEventListener('mouseup', handleMouseUp)
}

/**
 * 鼠标移动：更新临时连线终点
 */
const handleMouseMove = (e: MouseEvent) => {
  if (!dragging.value || !svgRef.value) return

  // 将浏览器鼠标坐标转换为 SVG 内部坐标
  const rect = svgRef.value.getBoundingClientRect()
  const x = e.clientX - rect.left
  const y = e.clientY - rect.top
  tempEndPos.value = { x, y }
}

/**
 * 鼠标松开：判定是否创建/解除连线
 */
const handleMouseUp = (e: MouseEvent) => {
  if (!dragging.value || !svgRef.value) {
    cleanDragState()
    return
  }

  // 计算鼠标在 SVG 内的坐标
  const rect = svgRef.value.getBoundingClientRect()
  const mouseX = e.clientX - rect.left
  const mouseY = e.clientY - rect.top

  // 查找命中的目标节点（阈值 10px）
  const targetDot = dots.value.find(dot => {
    const dx = dot.x - mouseX
    const dy = dot.y - mouseY
    return Math.sqrt(dx * dx + dy * dy) <= 10
  })

  // 命中有效目标且不是自身时，切换连线状态
  if (targetDot && targetDot.id !== startDotId.value) {
    // 过滤并排序涉及到要操作连线的两个节点
    const [action1, action2] = props.actions
      .filter(
        item =>
          item.action_id === targetDot.id || item.action_id === startDotId.value
      )
      .sort((a, b) => a.sort_num - b.sort_num)

    if (action1.has_data_permission && !action2.has_data_permission) {
      ElMessage.error('非数据权限节点不可依赖于数据权限节点')
    }

    // 优先级高低
    const high = action1.action_id
    const low = action2.action_id

    const existIndex = linkList.value.findIndex(
      link => link.high === high && link.low === low
    )

    if (existIndex > -1) {
      // 已存在 → 删除（解除连接）
      linkList.value.splice(existIndex, 1)
    } else {
      // 不存在 → 添加（创建连接）
      linkList.value.push({ high, low })
    }
  }

  cleanDragState()
}

/**
 * 清理拖拽状态与全局事件
 */
const cleanDragState = () => {
  dragging.value = false
  startDotId.value = null
  tempEndPos.value = null
  document.removeEventListener('mousemove', handleMouseMove)
  document.removeEventListener('mouseup', handleMouseUp)
}

/**
 * 组件卸载时兜底清理事件
 */
onUnmounted(() => {
  cleanDragState()
})
</script>

<template>
  <svg ref="svgRef" width="100px" :height="height" @mouseleave="handleMouseUp">
    <!-- 连接线层 -->
    <g class="link-group">
      <!-- 正式连线 -->
      <path
        v-for="item in linkPaths"
        :key="item.key"
        :d="item.d"
        stroke="#67c23a"
        stroke-width="1.5"
        fill="none"
        class="link-path"
      />
      <!-- 拖拽临时连线（虚线样式区分） -->
      <path
        v-if="dragging"
        :d="tempLinkPath"
        stroke="#409EFF"
        stroke-width="1.5"
        stroke-dasharray="4 2"
        fill="none"
      />
    </g>

    <!-- 节点圆点层 -->
    <g class="node-group">
      <circle
        v-for="dot in dots"
        :key="dot.id"
        :style="{ transform: `translate(${dot.x}px, ${dot.y}px)` }"
        cx="0"
        cy="0"
        r="5"
        fill="#409EFF"
        class="node-circle"
        @mousedown="e => handleMouseDown(e, dot.id)"
      />
    </g>
  </svg>
</template>

<style scoped>
.node-circle {
  transition: transform 0.3s ease;
  cursor: pointer;
}

.link-path {
  transition: all 0.3s ease;
  pointer-events: none;
}
</style>
