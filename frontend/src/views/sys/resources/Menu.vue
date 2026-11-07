<script lang="ts" setup>
import { Search, Refresh } from '@element-plus/icons-vue'
import {
  ElMessage,
  ElMessageBox,
  FormInstance,
  FormRules
} from 'element-plus/es'
import { computed, onMounted, reactive, ref } from 'vue'
import type {
  Menu,
  MenuFormData,
  MenuSortFormDataItem,
  MenuSortItem
} from '@/types/menu'
import {
  getMenuTree,
  newMenu,
  updateMenu,
  deleteMenu,
  sortMenu
} from '@/api/menu'
import { VueDraggable } from 'vue-draggable-plus'
import dgCard from '@/components/dg-card.vue'
import { MenuType } from '@/types/enums'
import { Icon } from '@iconify/vue'

/**
 * 从后端返回的真实数据
 */
const menuTree = ref<Menu[]>([])

/**
 * 经过过滤后实际展示在表格里的数据
 */
const menuShow = ref<Menu[]>([])

/**
 * 添加child_count字段
 */
const addChildCount = (node: Menu): void => {
  node.child_count = node.children ? node.children.length : 0
  if (node.children && node.children.length > 0) {
    node.children.forEach(child => addChildCount(child))
  }
}

const getMenuData = async () => {
  menuTree.value = await getMenuTree()
  menuTree.value.forEach(node => addChildCount(node))
  menuShow.value = menuTree.value
}

onMounted(async () => {
  await getMenuData()
})

/// 修改这个函数，要求是：和menuForm的menu_id进行比较，如果匹配，也禁用
/// 如果是因为menu_id的匹配导致的禁用，传递给所有的子菜单也禁用
const cascaderData = computed(() => {
  const markDisabled = (nodes: Menu[]): Menu[] =>
    nodes.map(node => {
      const selfMatch = node.menu_id === menuForm.menu_id
      const children = node.children
        ? markDisabled(node.children).map(child => ({
            ...child,
            disabled: child.disabled || selfMatch
          }))
        : undefined
      return {
        ...node,
        disabled: node.type !== MenuType.Menu || selfMatch,
        children
      }
    })
  return markDisabled(menuShow.value)
})

/**
 * 更新时当前菜单项为目录且包含子菜单项时，禁用页面和外链类型选择
 */
const lockTypeOnDirectory = computed(
  () =>
    dialogType.value === DialogType.UPDATE &&
    menuForm.type === MenuType.Menu &&
    (menuForm.child_count ?? 0) > 0
)

/**
 * 查询表单（并不请求接口，前端过滤展示数据）
 */
const queryForm = ref<FormInstance>()

/**
 * 查询表单参数
 */
const queryParams = reactive<{ name: string }>({
  name: ''
})

/**
 * 过滤菜单树：返回包含 keyword 的节点及其所有父级（路径上所有祖先），并维持树结构
 * @param nodes 原始菜单树
 * @param keyword 搜索关键词（模糊匹配 name 字段）
 * @returns 过滤后的菜单树（只含匹配节点 + 必要祖先 + 必要后代子树）
 */
function filterMenuTree(nodes: Menu[], keyword: string): Menu[] {
  if (!keyword.trim())
    return nodes.map(menu => ({
      ...menu,
      children: menu.children ? menu.children.map(c => ({ ...c })) : undefined
    }))

  const collectNodes = (
    menus: Menu[],
    keyword: string
  ): {
    allNodes: Map<number, Menu>
    matchedNodes: Menu[]
  } => {
    const flatten = (nodes: Menu[]): Menu[] =>
      nodes.flatMap(menu => [
        menu,
        ...(menu.children?.length ? flatten(menu.children) : [])
      ])

    const allList = flatten(menus)
    const allNodes = new Map(allList.map(d => [d.menu_id, d] as const))
    const matchedNodes = allList.filter(d =>
      d.name.toLowerCase().includes(keyword)
    )

    return { allNodes, matchedNodes }
  }
  const { allNodes, matchedNodes } = collectNodes(nodes, keyword)

  if (matchedNodes.length === 0) return []

  const includedIds = new Set<number>()
  const ancestors = (node: Menu): void => {
    let curr: Menu | undefined = node
    while (curr) {
      includedIds.add(curr.menu_id)
      curr = allNodes.get(curr.parent_id!) || undefined
    }
  }
  matchedNodes.forEach(ancestors)

  const nodeMap = new Map<number, Menu>(
    Array.from(includedIds, id => [id, { ...allNodes.get(id)! }] as const)
  )

  nodeMap.forEach((node, id) => {
    const original = allNodes.get(id)
    if (original?.children?.length) {
      node.children = original.children
        .filter(child => includedIds.has(child.menu_id))
        .map(child => nodeMap.get(child.menu_id)!)
    }
  })

  return Array.from(nodeMap.values()).filter(
    node => node.parent_id == null || !includedIds.has(node.parent_id)
  )
}

/**
 * 过滤数据
 */
const execMenuFilter = () => {
  const nodes = menuTree.value
  const keyword = queryParams.name.trim()
  const filteredResult = filterMenuTree(nodes, keyword)
  menuShow.value = filteredResult
}

/**
 * 重置查询表单数据
 */
const resetQuery = () => {
  queryParams.name = ''
  menuShow.value = menuTree.value
}

/**
 * 是否展示新增/更新对话框
 */
const dialogVisible = ref(false)

/**
 * 定义对话框类型枚举
 */
enum DialogType {
  ADD = 0, // 新增
  UPDATE = 1 // 更新
}

/**
 * 对话框类型
 */
const dialogType = ref<DialogType>(DialogType.ADD)

/**
 * 菜单表单数据
 */
const menuForm = reactive<MenuFormData>({
  type: MenuType.Menu,
  name: '',
  path: '',
  icon: '',
  component: '',
  menu_id: undefined,
  version: undefined
})

/**
 * 菜单表单实例
 */
const menuFormRef = ref<FormInstance>()

/**
 * 根据菜单类型动态生成校验规则
 */
const getRules = (): FormRules<MenuFormData> => {
  const baseRules: FormRules<MenuFormData> = {
    name: [{ required: true, message: '请输入菜单名称', trigger: 'blur' }],
    type: [{ required: true, message: '请选择菜单类型', trigger: 'change' }]
  }

  // 更新时 type 不可修改，不需要校验
  if (dialogType.value === DialogType.ADD) {
    return baseRules
  }

  return baseRules
}

const rules = ref<FormRules<MenuFormData>>(getRules())

/**
 * 新增菜单按钮
 */
const newMenuBtn = () => {
  menuForm.type = MenuType.Menu
  menuForm.name = ''
  menuForm.path = ''
  menuForm.icon = ''
  menuForm.component = ''
  menuForm.menu_id = undefined
  menuForm.version = undefined
  menuForm.parent_id = undefined

  dialogType.value = DialogType.ADD
  dialogVisible.value = true
}

/**
 * 取消新增/更新
 */
const cancel = () => {
  dialogVisible.value = false
}

/**
 * 根据菜单id查找到原始数据
 */
const findOriginal = (menuList: Menu[], menuId?: number): Menu | undefined => {
  if (menuId === undefined || typeof menuId !== 'number') {
    return undefined
  }

  const deepFind = (list: Menu[]): Menu | undefined =>
    list.reduce<Menu | undefined>((found, menu) => {
      if (found) return found
      if (menu.menu_id === menuId) return menu
      return deepFind(menu.children ?? [])
    }, undefined)

  return deepFind(menuList)
}

/**
 * 处理新增菜单
 */
const handleAddMenu = async (data: MenuFormData): Promise<boolean> => {
  if (!data.name.trim()) {
    ElMessage.error('菜单名称不可为空')
    return false
  }

  if (data.type === MenuType.Page) {
    if (!data.path?.trim()) {
      ElMessage.error('页面类型必须有访问路径')
      return false
    }
    if (!data.component?.trim()) {
      ElMessage.error('页面类型必须有组件路径')
      return false
    }
  } else if (data.type === MenuType.OutLink) {
    if (!data.path?.trim()) {
      ElMessage.error('外链类型必须有访问路径')
      return false
    }
    if (!data.path.startsWith('http://') && !data.path.startsWith('https://')) {
      ElMessage.error('外链类型路径必须是完整URL')
      return false
    }
  }

  await newMenu({
    name: data.name,
    type: data.type,
    path: data.type !== MenuType.Menu ? data.path : undefined,
    icon: data.icon,
    component: data.type === MenuType.Page ? data.component : undefined,
    parent_id: data.parent_id
  })
  ElMessage.success('新增成功')
  dialogVisible.value = false
  await getMenuData()
  resetQuery()
  return true
}

/**
 * 处理更新菜单
 */
const handleUpdateMenu = async (
  data: MenuFormData,
  tree: Menu[]
): Promise<boolean> => {
  const original = findOriginal(tree, data.menu_id)

  if (!original) {
    ElMessage.error('更新失败')
    console.error('没有查找到原始数据')
    return false
  }

  // 菜单类型不可修改
  if (data.type !== original.type) {
    ElMessage.error('菜单类型不可修改')
    return false
  }

  if (
    data.name === original.name &&
    data.icon === original.icon &&
    data.path === original.path &&
    data.component === original.component
  ) {
    ElMessage.warning('数据未发生变化，无需更新')
    return false
  }

  await updateMenu(original.menu_id!, {
    version: original.version,
    name: data.name,
    icon: data.icon || null,
    path: data.path || null,
    component: data.component || null
  })
  ElMessage.success('更新成功')
  dialogVisible.value = false
  await getMenuData()
  resetQuery()
  return true
}

/**
 * 提交更新/新增
 */
const submit = async (formEl?: FormInstance) => {
  if (!(await formEl?.validate())) return

  // 重新计算规则
  rules.value = getRules()

  const actionHandlers = {
    [DialogType.ADD]: () => handleAddMenu(menuForm),
    [DialogType.UPDATE]: () => handleUpdateMenu(menuForm, menuTree.value)
  }

  await actionHandlers[dialogType.value]?.()
}

/**
 * 当前正在排序的父菜单ID（undefined表示正在排序顶级菜单）
 */
const currentParentId = ref<number | undefined>(undefined)

/**
 * 排序数据
 */
const sortableData = ref<MenuSortItem[]>([])

/**
 * 排序对话框是否展示
 */
const sortDialogVisible = ref(false)

/**
 * 根据id查找菜单
 */
const findMenuById = (menuList: Menu[], menuId: number): Menu | undefined => {
  for (const item of menuList) {
    if (item.menu_id === menuId) {
      return item
    }
    if (item.children && item.children.length > 0) {
      const result = findMenuById(item.children, menuId)
      if (result) return result
    }
  }
  return undefined
}

/**
 * 处理排序子菜单按钮逻辑
 *
 * @param parentId 父菜单ID（可选）
 * @param menuData 原始菜单数据
 * @returns 排序数据 + 弹窗显示状态
 */
const getSortSubMenuData = (
  parentId: number | undefined,
  menuData: Menu[]
): { data: MenuSortItem[]; visible: boolean } => {
  // 无父ID：处理顶级菜单
  if (parentId === undefined) {
    const data = menuData.map(item => ({
      menu_id: item.menu_id,
      name: item.name,
      version: item.version
    }))
    return { data, visible: true }
  }

  // 有父ID：查找父菜单并处理子菜单
  const targetMenu = findMenuById(menuData, parentId)
  const data = targetMenu?.children
    ? targetMenu.children.map(item => ({
        menu_id: item.menu_id,
        name: item.name,
        version: item.version
      }))
    : []

  return { data, visible: true }
}

/**
 * 排序子菜单按钮
 */
const sortSubMenuBtn = (parentId?: number) => {
  const { data, visible } = getSortSubMenuData(parentId, menuTree.value)
  currentParentId.value = parentId
  sortableData.value = data
  sortDialogVisible.value = visible
}

/**
 * 取消排序
 */
const sortCancel = () => {
  sortDialogVisible.value = false
}

/**
 * 提交排序
 */
const sortSubmit = async () => {
  const menus = sortableData.value.map(
    item =>
      ({
        menu_id: item.menu_id,
        version: item.version
      }) as MenuSortFormDataItem
  )
  await sortMenu(currentParentId.value, menus)

  ElMessage.success('排序成功')
  await getMenuData()
  sortDialogVisible.value = false
}

/**
 * 表格内更新菜单按钮
 */
const updateMenuBtn = (row: Menu) => {
  dialogType.value = DialogType.UPDATE
  menuForm.menu_id = row.menu_id
  menuForm.parent_id = row.parent_id
  menuForm.version = row.version
  menuForm.type = row.type
  menuForm.child_count = row.child_count
  menuForm.name = row.name
  menuForm.path = row.path || ''
  menuForm.icon = row.icon || ''
  menuForm.component = row.component || ''
  rules.value = getRules()
  dialogVisible.value = true
}

/**
 * 表格内删除菜单按钮
 */
const deleteMenuBtn = async (menuId: number) => {
  const menuToDelete = findOriginal(menuShow.value, menuId)
  if (menuToDelete === undefined) {
    ElMessage.warning('菜单不存在，无法删除')
    return
  }
  if (menuToDelete.children && menuToDelete.children.length > 0) {
    ElMessage.warning('存在子菜单，无法删除')
    return
  }
  ElMessageBox.confirm(`请确认是否要删除 ${menuToDelete.name} 菜单`).then(
    async () => {
      await deleteMenu(menuToDelete.menu_id, menuToDelete.version)
      ElMessage.success('删除成功')
      await getMenuData()
      resetQuery()
    }
  )
}
</script>

<template>
  <!-- 过滤数据表单 -->
  <dg-card>
    <el-form
      :model="queryParams"
      ref="queryForm"
      :inline="true"
      label-width="68px"
    >
      <el-form-item label="菜单名称">
        <el-input v-model="queryParams.name" placeholder="菜单名称" clearable />
      </el-form-item>
      <el-form-item>
        <el-button
          type="primary"
          :icon="Search"
          size="small"
          @click="execMenuFilter"
          >搜索</el-button
        >
        <el-button :icon="Refresh" size="small" @click="resetQuery"
          >重置</el-button
        >
      </el-form-item>
    </el-form>
  </dg-card>
  <!-- 主体数据展示 -->
  <dg-card>
    <!-- 按钮 -->
    <el-row :gutter="10">
      <el-col :span="1.5">
        <el-button @click="newMenuBtn">新增菜单</el-button>
        <el-button
          :disabled="menuTree.length <= 1"
          @click="sortSubMenuBtn(undefined)"
          >排序</el-button
        >
      </el-col>
    </el-row>
    <!-- 表格 -->
    <el-table :data="menuShow" row-key="menu_id" default-expand-all>
      <el-table-column prop="name" label="菜单名称" min-width="80px">
        <template v-slot="{ row }">
          <el-icon v-if="row.icon"><Icon :icon="row.icon" /></el-icon>
          {{ row.name }}
        </template>
      </el-table-column>
      <el-table-column prop="type" label="类型" width="80">
        <template v-slot="{ row }">
          <el-tag v-if="row.type === MenuType.Menu"> 目录 </el-tag>
          <el-tag v-if="row.type === MenuType.Page" type="success">
            页面
          </el-tag>
          <el-tag v-if="row.type === MenuType.OutLink" type="danger">
            外链
          </el-tag>
        </template>
      </el-table-column>
      <el-table-column prop="path" label="访问路径" min-width="150px" />
      <el-table-column prop="component" label="组件路径" min-width="150px" />
      <el-table-column fixed="right" label="操作" width="240">
        <template v-slot="{ row }">
          <el-button plain type="primary" @click="updateMenuBtn(row)"
            >更新</el-button
          >
          <el-button
            v-if="row.child_count > 0"
            plain
            type="primary"
            :disabled="row.child_count === 1"
            @click="sortSubMenuBtn(row.menu_id)"
            >子菜单排序</el-button
          >
          <el-button
            v-else
            plain
            type="danger"
            @click="deleteMenuBtn(row.menu_id)"
            >删除</el-button
          >
        </template>
      </el-table-column>
    </el-table>
  </dg-card>
  <!-- 新增/更新 对话框 -->
  <el-dialog
    :title="dialogType ? '更新菜单' : '新增菜单'"
    v-model="dialogVisible"
    width="500px"
  >
    <el-form
      ref="menuFormRef"
      :rules="rules"
      :model="menuForm"
      label-width="80px"
    >
      <el-form-item label="所属菜单" prop="parent_id">
        <el-cascader
          v-model="menuForm.parent_id"
          :options="cascaderData"
          clearable
          placeholder="请选择父菜单，可为空"
          :props="{
            checkStrictly: true,
            emitPath: false,
            value: 'menu_id',
            label: 'name',
            children: 'children'
          }"
        />
      </el-form-item>
      <el-form-item label="菜单类型" prop="type">
        <el-radio-group v-model="menuForm.type">
          <el-radio :value="MenuType.Menu">目录</el-radio>
          <el-radio :value="MenuType.Page" :disabled="lockTypeOnDirectory">
            页面
          </el-radio>
          <el-radio :value="MenuType.OutLink" :disabled="lockTypeOnDirectory"
            >外链</el-radio
          >
        </el-radio-group>
      </el-form-item>
      <el-form-item label="菜单名称" prop="name">
        <el-input v-model="menuForm.name" placeholder="请输入菜单名称" />
      </el-form-item>
      <el-form-item
        v-if="dialogType === DialogType.ADD"
        label="图标"
        prop="icon"
      >
        <el-input v-model="menuForm.icon" placeholder="请输入图标名称" />
      </el-form-item>
      <el-form-item
        v-if="menuForm.type !== MenuType.Menu"
        label="访问路径"
        prop="path"
      >
        <el-input
          v-model="menuForm.path"
          :placeholder="
            menuForm.type === MenuType.OutLink
              ? '请输入完整URL地址'
              : '请输入访问路径'
          "
        />
      </el-form-item>
      <el-form-item
        v-if="menuForm.type === MenuType.Page"
        label="组件路径"
        prop="component"
      >
        <el-input
          v-model="menuForm.component"
          placeholder="请输入组件路径，如 /sys/resources/Menu"
        />
      </el-form-item>
    </el-form>
    <template #footer>
      <el-button @click="cancel()">取 消</el-button>
      <el-button type="primary" @click="submit(menuFormRef)">提 交</el-button>
    </template>
  </el-dialog>
  <!-- 子菜单排序对话框 -->
  <el-dialog title="排序" v-model="sortDialogVisible" width="400px">
    <VueDraggable ref="drag" v-model="sortableData">
      <div class="sort-item" v-for="item in sortableData">
        {{ item.name }}
      </div>
    </VueDraggable>
    <template #footer>
      <el-button @click="sortCancel()">取 消</el-button>
      <el-button type="primary" @click="sortSubmit()">提 交</el-button>
    </template>
  </el-dialog>
</template>

<style scoped lang="scss">
.query {
  padding-bottom: 0;
}

.dg-card {
  margin-bottom: 20px;
}

:deep(.el-input) {
  width: 186px;
}

.el-table {
  margin-top: 15px;
}

.sort-item {
  user-select: none;
  width: fit-content;
  height: 30px;
  line-height: 30px;
  margin-bottom: 10px;
  margin-left: 20px;
  padding: 0 15px;
  color: #409eff;
  border: 1px solid #409eff;
  border-radius: 10px;
  cursor: pointer;
}

:deep(.sortable-chosen) {
  cursor: pointer;
}
</style>
