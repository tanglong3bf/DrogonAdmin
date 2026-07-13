<script lang="ts" setup>
import { Search, Refresh } from '@element-plus/icons-vue'
import {
  ElMessage,
  ElMessageBox,
  FormInstance,
  FormRules
} from 'element-plus/es'
import { onMounted, reactive, ref } from 'vue'
import type {
  Module,
  ModuleFormData,
  ModuleSortItem,
  Action
} from '@/types/module'
import {
  getModuleTree,
  newModule,
  updateModule,
  deleteModule,
  sortModule
} from '@/api/module'
import { VueDraggable } from 'vue-draggable-plus'

/**
 * 从后端返回的真实数据
 */
const moduleTree = ref<Module[]>([])

/**
 * 经过过滤后实际展示在表格里的数据
 */
const moduleShow = ref<Module[]>([])

/**
 * 添加child_count字段
 */
const addChildCount = (node: Module): void => {
  node.child_count = node.children ? node.children.length : 0
  if (node.children && node.children.length > 0) {
    node.children.forEach(child => addChildCount(child))
  }
}

const getModuleData = async () => {
  moduleTree.value = await getModuleTree()
  moduleTree.value.forEach(node => addChildCount(node))
  moduleShow.value = moduleTree.value
}

onMounted(async () => {
  await getModuleData()
})

/**
 * 查询表单参数
 */
const queryParams = reactive<{ name: string }>({
  name: ''
})

/**
 * 过滤数据
 */
const execModuleFilter = () => {
  const nodes = moduleTree.value
  const keyword = queryParams.name.trim()
  const filteredResult = filterModuleTree(nodes, keyword)
  moduleShow.value = filteredResult
}

/**
 * 重置查询表单数据
 */
const resetQuery = () => {
  queryParams.name = ''
  moduleShow.value = moduleTree.value
}

/**
 * 过滤模块树：返回包含 keyword 的节点及其所有父级（路径上所有祖先），并维持树结构
 * @param nodes 原始模块树
 * @param keyword 搜索关键词（模糊匹配 name 字段）
 * @returns 过滤后的模块树（只含匹配节点 + 必要祖先 + 必要后代子树）
 */
function filterModuleTree(nodes: Module[], keyword: string): Module[] {
  if (!keyword.trim())
    return nodes.map(module => ({
      ...module,
      children: module.children
        ? module.children.map(c => ({ ...c }))
        : undefined
    }))

  const collectNodes = (
    modules: Module[],
    keyword: string
  ): {
    allNodes: Map<number, Module>
    matchedNodes: Module[]
  } => {
    const flatten = (nodes: Module[]): Module[] =>
      nodes.flatMap(module => [
        module,
        ...(module.children?.length ? flatten(module.children) : [])
      ])

    const allList = flatten(modules)
    const allNodes = new Map(allList.map(m => [m.module_id, m] as const))
    const matchedNodes = allList.filter(m =>
      m.name.toLowerCase().includes(keyword)
    )

    return { allNodes, matchedNodes }
  }
  const { allNodes, matchedNodes } = collectNodes(nodes, keyword)

  if (matchedNodes.length === 0) return []

  const includedIds = new Set<number>()
  const ancestors = (node: Module): void => {
    let curr: Module | undefined = node
    while (curr) {
      includedIds.add(curr.module_id)
      curr = allNodes.get(curr.parent_id!) || undefined
    }
  }
  matchedNodes.forEach(ancestors)

  const nodeMap = new Map<number, Module>(
    Array.from(includedIds, id => [id, { ...allNodes.get(id)! }] as const)
  )

  nodeMap.forEach((node, id) => {
    const original = allNodes.get(id)
    if (original?.children?.length) {
      node.children = original.children
        .filter(child => includedIds.has(child.module_id))
        .map(child => nodeMap.get(child.module_id)!)
    }
  })

  return Array.from(nodeMap.values()).filter(
    node => node.parent_id == null || !includedIds.has(node.parent_id)
  )
}
/**
 * 模块表单数据
 */
const module = reactive<ModuleFormData>({
  module_id: undefined,
  name: '',
  description: '',
  parent_id: undefined
})

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
 * 模块表单实例
 */
const moduleForm = ref<FormInstance>()

/**
 * 表单验证规则
 */
const rules = reactive<FormRules<ModuleFormData>>({
  name: [{ required: true, message: '请输入模块名称', trigger: 'blur' }]
})

/**
 * 新增模块按钮
 */
const newModuleBtn = () => {
  module.module_id = undefined
  module.name = ''
  module.description = undefined
  module.parent_id = undefined

  dialogType.value = DialogType.ADD
  dialogVisible.value = true
}

/**
 * 取消更新/新增
 */
const cancel = () => {
  dialogVisible.value = false
}

/**
 * 根据模块id查找到原始数据
 */
const findOriginal = (
  moduleList: Module[],
  moduleId?: number
): Module | undefined => {
  if (moduleId === undefined || typeof moduleId !== 'number') {
    return undefined
  }

  const deepFind = (list: Module[]): Module | undefined =>
    list.reduce<Module | undefined>((found, module) => {
      if (found) return found
      if (module.module_id === moduleId) return module
      return deepFind(module.children ?? [])
    }, undefined)

  return deepFind(moduleList)
}

/**
 * 处理新增模块
 */
const handleAddModule = async (module: ModuleFormData): Promise<boolean> => {
  if (!module.name.trim()) {
    ElMessage.error('模块名称不可为空')
    return false
  }
  if (module.description === null) {
    module.description = undefined
  }
  await newModule(module.name, module.description, module.parent_id)
  dialogVisible.value = false
  await getModuleData()
  resetQuery()
  return true
}

/**
 * 处理更新模块
 */
const handleUpdateModule = async (
  module: ModuleFormData,
  data: Module[]
): Promise<boolean> => {
  const original = findOriginal(data, module.module_id)

  if (!original) {
    ElMessage.error('更新失败')
    console.error('没有查找到原始数据')
    return false
  }

  if (
    module.name === original.name &&
    module.description === original.description
  ) {
    ElMessage.warning('数据未发生变化，无需更新')
    return false
  }

  await updateModule(
    module.module_id!,
    module.name,
    module.description === '' ? null : module.description
  )
  ElMessage.success('更新成功')
  original.name = module.name
  original.description =
    module.description === null ? undefined : module.description
  dialogVisible.value = false
  return true
}

/**
 * 提交更新/新增
 */
const submit = async (formEl?: FormInstance) => {
  if (!(await formEl?.validate())) return

  const actionHandlers = {
    [DialogType.ADD]: () => handleAddModule(module),
    [DialogType.UPDATE]: () => handleUpdateModule(module, moduleTree.value)
  }

  await actionHandlers[dialogType.value]?.()
}

/**
 * 当前正在排序的父模块ID（undefined表示正在排序顶级模块）
 */
const currentParentId = ref<number | undefined>(undefined)

/**
 * 排序数据
 */
const sortableData = ref<ModuleSortItem[]>([])

/**
 * 排序对话框是否展示
 */
const sortDialogVisible = ref(false)

/**
 * 根据id查找模块
 */
const findModuleById = (
  moduleList: Module[],
  module_id: number
): Module | undefined => {
  for (const item of moduleList) {
    if (item.module_id === module_id) {
      return item
    }
    if (item.children && item.children.length > 0) {
      const result = findOriginal(item.children, module_id)
      if (result) return result
    }
  }
  return undefined
}

/**
 * 处理排序子模块按钮逻辑
 *
 * @param parentId 父模块ID（可选）
 * @param moduleData 原始模块数据
 * @returns 排序数据 + 弹窗显示状态
 */
const getSortSubModuleData = (
  parentId: number | undefined,
  moduleData: Module[]
): { data: ModuleSortItem[]; visible: boolean } => {
  // 无父ID：处理顶级模块
  if (parentId === undefined) {
    const data = moduleData.map(item => ({
      module_id: item.module_id,
      name: item.name,
      sort_num: item.sort_num
    }))
    return { data, visible: true }
  }

  // 有父ID：查找父模块并处理子模块
  const targetModule = findModuleById(moduleData, parentId)
  const data = targetModule?.children
    ? targetModule.children.map(item => ({
        module_id: item.module_id,
        name: item.name,
        sort_num: item.sort_num
      }))
    : []

  return { data, visible: true }
}

/**
 * 排序子模块按钮
 */
const sortSubModuleBtn = (parentId?: number) => {
  const { data, visible } = getSortSubModuleData(parentId, moduleTree.value)
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
  const moduleIds = sortableData.value.map(item => {
    return item.module_id
  })
  await sortModule(currentParentId.value, moduleIds)

  ElMessage.success('排序成功')
  await getModuleData()
  sortDialogVisible.value = false
}

/**
 * 表格内更新模块按钮
 */
const updateModuleBtn = (row: Module) => {
  dialogType.value = DialogType.UPDATE
  module.module_id = row.module_id
  module.name = row.name
  module.description = row.description
  module.parent_id = row.parent_id
  dialogVisible.value = true
}

/**
 * 表格内删除模块按钮
 */
const deleteModuleBtn = async (moduleId: number) => {
  const moduleToDelete = findOriginal(moduleShow.value, moduleId)
  if (moduleToDelete === undefined) {
    ElMessage.warning('模块不存在，无法删除')
    return
  }
  if (moduleToDelete.children && moduleToDelete.children.length > 0) {
    ElMessage.warning('存在子模块，无法删除')
    return
  }
  ElMessageBox.confirm(`请确认是否要删除 ${moduleToDelete.name} 模块`).then(
    async () => {
      await deleteModule(moduleToDelete.module_id)
      ElMessage.success('删除成功')
      await getModuleData()
      queryParams.name = ''
    }
  )
}

const actionDialogVisible = ref(false)

const sortableAction = ref<Action[] | undefined>([])

const assignAction = (module_id: number) => {
  const original: Module | undefined = findOriginal(moduleTree.value, module_id)
  sortableAction.value = original?.actions?.map(item => ({
    action_id: item.action_id,
    name: item.name,
    sort_num: item.sort_num,
    has_data_permission: item.has_data_permission
  }))
  actionDialogVisible.value = true
}

const assignCancel = () => {
  actionDialogVisible.value = false
}

const assignSubmit = () => {
  actionDialogVisible.value = false
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
      <el-form-item label="模块名称">
        <el-input v-model="queryParams.name" placeholder="模块名称" clearable />
      </el-form-item>
      <el-form-item>
        <el-button
          type="primary"
          :icon="Search"
          size="small"
          @click="execModuleFilter"
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
        <el-button @click="newModuleBtn">新增模块</el-button>
        <el-button
          :disabled="moduleTree.length <= 1"
          @click="sortSubModuleBtn(undefined)"
          >排序</el-button
        >
      </el-col>
    </el-row>
    <!-- 表格 -->
    <el-table :data="moduleShow" row-key="module_id" default-expand-all>
      <el-table-column prop="name" label="模块名称" min-width="120px" />
      <el-table-column fixed="right" label="操作">
        <template v-slot="{ row }">
          <el-button plain type="primary" @click="updateModuleBtn(row)"
            >更新</el-button
          >
          <el-button plain type="warning" @click="assignAction(row.module_id)"
            >设置功能</el-button
          >
          <el-button
            v-if="row.child_count > 0"
            plain
            type="primary"
            :disabled="row.child_count === 1"
            @click="sortSubModuleBtn(row.module_id)"
            >子模块排序</el-button
          >
          <el-button
            v-else
            plain
            type="danger"
            @click="deleteModuleBtn(row.module_id)"
            >删除</el-button
          >
        </template>
      </el-table-column>
    </el-table>
  </dg-card>
  <!-- 新增/更新 对话框 -->
  <el-dialog
    :title="dialogType ? '更新模块' : '新增模块'"
    v-model="dialogVisible"
    width="400px"
  >
    <el-form ref="moduleForm" :rules="rules" :model="module" label-width="80px">
      <el-form-item label="模块名称" prop="name">
        <el-input v-model="module.name" placeholder="请输入模块名称" />
      </el-form-item>
      <el-form-item label="模块描述" prop="description">
        <el-input
          v-model="module.description"
          placeholder="请输入模块描述"
          type="textarea"
        />
      </el-form-item>
      <el-form-item
        v-if="dialogType === DialogType.ADD"
        label="所属模块"
        prop="parent_id"
      >
        <el-cascader
          v-model="module.parent_id"
          :options="moduleTree"
          clearable
          placeholder="请选择父模块，可为空"
          :props="{
            checkStrictly: true,
            emitPath: false,
            value: 'module_id',
            label: 'name',
            children: 'children'
          }"
        />
      </el-form-item>
    </el-form>
    <template #footer>
      <el-button @click="cancel()">取 消</el-button>
      <el-button type="primary" @click="submit(moduleForm)">提 交</el-button>
    </template>
  </el-dialog>
  <!-- 子模块排序对话框 -->
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
  <!-- 功能设置对话框 -->
  <el-dialog title="设置功能" v-model="actionDialogVisible" width="400px">
    {{ sortableAction }}
    <template #footer>
      <el-button @click="assignCancel()">取 消</el-button>
      <el-button type="primary" @click="assignSubmit()">提 交</el-button>
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
