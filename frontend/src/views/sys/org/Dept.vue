<script lang="ts" setup>
import { Search, Refresh } from '@element-plus/icons-vue'
import {
  ElMessage,
  ElMessageBox,
  FormInstance,
  FormRules
} from 'element-plus/es'
import { onMounted, reactive, ref } from 'vue'
import type { Department, DeptFormData, DeptSortItem } from '@/types/department'
import {
  getDeptTree,
  newDept,
  updateDept,
  deleteDept,
  sortDept
} from '@/api/department'
import { VueDraggable } from 'vue-draggable-plus'
import dgCard from '@/components/dg-card.vue'

/**
 * 从后端返回的真实数据
 */
const deptTree = ref<Department[]>([])

/**
 * 经过过滤后实际展示在表格里的数据
 */
const deptShow = ref<Department[]>([])

/**
 * 添加child_count字段
 */
const addChildCount = (node: Department): void => {
  node.child_count = node.children ? node.children.length : 0
  if (node.children && node.children.length > 0) {
    node.children.forEach(child => addChildCount(child))
  }
}

const getDeptData = async () => {
  deptTree.value = await getDeptTree()
  deptTree.value.forEach(node => addChildCount(node))
  deptShow.value = deptTree.value
}

onMounted(async () => {
  await getDeptData()
})

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
 * 过滤部门树：返回包含 keyword 的节点及其所有父级（路径上所有祖先），并维持树结构
 * @param nodes 原始部门树
 * @param keyword 搜索关键词（模糊匹配 name 字段）
 * @returns 过滤后的部门树（只含匹配节点 + 必要祖先 + 必要后代子树）
 */
function filterDeptTree(nodes: Department[], keyword: string): Department[] {
  if (!keyword.trim())
    return nodes.map(dept => ({
      ...dept,
      children: dept.children ? dept.children.map(c => ({ ...c })) : undefined
    }))

  const collectNodes = (
    depts: Department[],
    keyword: string
  ): {
    allNodes: Map<number, Department>
    matchedNodes: Department[]
  } => {
    const flatten = (nodes: Department[]): Department[] =>
      nodes.flatMap(dept => [
        dept,
        ...(dept.children?.length ? flatten(dept.children) : [])
      ])

    const allList = flatten(depts)
    const allNodes = new Map(allList.map(d => [d.dept_id, d] as const))
    const matchedNodes = allList.filter(d =>
      d.name.toLowerCase().includes(keyword)
    )

    return { allNodes, matchedNodes }
  }
  const { allNodes, matchedNodes } = collectNodes(nodes, keyword)

  if (matchedNodes.length === 0) return []

  const includedIds = new Set<number>()
  const ancestors = (node: Department): void => {
    let curr: Department | undefined = node
    while (curr) {
      includedIds.add(curr.dept_id)
      curr = allNodes.get(curr.parent_id!) || undefined
    }
  }
  matchedNodes.forEach(ancestors)

  const nodeMap = new Map<number, Department>(
    Array.from(includedIds, id => [id, { ...allNodes.get(id)! }] as const)
  )

  nodeMap.forEach((node, id) => {
    const original = allNodes.get(id)
    if (original?.children?.length) {
      node.children = original.children
        .filter(child => includedIds.has(child.dept_id))
        .map(child => nodeMap.get(child.dept_id)!)
    }
  })

  return Array.from(nodeMap.values()).filter(
    node => node.parent_id == null || !includedIds.has(node.parent_id)
  )
}

/**
 * 过滤数据
 */
const execDeptFilter = () => {
  const nodes = deptTree.value
  const keyword = queryParams.name.trim()
  const filteredResult = filterDeptTree(nodes, keyword)
  deptShow.value = filteredResult
}

/**
 * 重置查询表单数据
 */
const resetQuery = () => {
  queryParams.name = ''
  deptShow.value = deptTree.value
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
 * 部门表单数据
 */
const department = reactive<DeptFormData>({
  parent_id: undefined,
  name: '',
  dept_id: undefined
})

/**
 * 部门表单实例
 */
const departmentForm = ref<FormInstance>()

/**
 * 表单验证规则
 */
const rules = reactive<FormRules<DeptFormData>>({
  name: [{ required: true, message: '请输入部门名称', trigger: 'blur' }]
})

/**
 * 新增部门按钮
 */
const newDepartmentBtn = () => {
  department.parent_id = undefined
  department.dept_id = undefined
  department.name = ''

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
 * 根据部门id查找到原始数据
 */
const findOriginal = (
  deptList: Department[],
  deptId?: number
): Department | undefined => {
  if (deptId === undefined || typeof deptId !== 'number') {
    return undefined
  }

  const deepFind = (list: Department[]): Department | undefined =>
    list.reduce<Department | undefined>((found, dept) => {
      if (found) return found
      if (dept.dept_id === deptId) return dept
      return deepFind(dept.children ?? [])
    }, undefined)

  return deepFind(deptList)
}

/**
 * 处理新增部门
 */
const handleAddDept = async (dept: DeptFormData): Promise<boolean> => {
  if (!dept.name.trim()) {
    ElMessage.error('部门名称不可为空')
    return false
  }
  await newDept(dept.name, dept.parent_id)
  dialogVisible.value = false
  await getDeptData()
  resetQuery()
  return true
}

/**
 * 处理更新部门
 */
const handleUpdateDept = async (
  dept: DeptFormData,
  data: Department[]
): Promise<boolean> => {
  const original = findOriginal(data, dept.dept_id)

  if (!original) {
    ElMessage.error('更新失败')
    console.error('没有查找到原始数据')
    return false
  }

  if (dept.name === original.name) {
    ElMessage.warning('数据未发生变化，无需更新')
    return false
  }

  await updateDept(original.dept_id!, dept.name)
  ElMessage.success('更新成功')
  original.name = dept.name // 仍需修改原对象（业务逻辑要求）
  dialogVisible.value = false
  return true
}

/**
 * 提交更新/新增
 */
const submit = async (formEl?: FormInstance) => {
  if (!(await formEl?.validate())) return

  const actionHandlers = {
    [DialogType.ADD]: () => handleAddDept(department),
    [DialogType.UPDATE]: () => handleUpdateDept(department, deptTree.value)
  }

  await actionHandlers[dialogType.value]?.()
}

/**
 * 当前正在排序的父部门ID（undefined表示正在排序顶级部门）
 */
const currentParentId = ref<number | undefined>(undefined)

/**
 * 排序数据
 */
const sortableData = ref<DeptSortItem[]>([])

/**
 * 排序对话框是否展示
 */
const sortDialogVisible = ref(false)

/**
 * 根据id查找部门
 */
const findDeptById = (
  deptList: Department[],
  dept_id: number
): Department | undefined => {
  for (const item of deptList) {
    if (item.dept_id === dept_id) {
      return item
    }
    if (item.children && item.children.length > 0) {
      const result = findOriginal(item.children, dept_id)
      if (result) return result
    }
  }
  return undefined
}

/**
 * 处理排序子部门按钮逻辑
 *
 * @param parentId 父部门ID（可选）
 * @param deptData 原始部门数据
 * @returns 排序数据 + 弹窗显示状态
 */
const getSortSubDeptData = (
  parentId: number | undefined,
  deptData: Department[]
): { data: DeptSortItem[]; visible: boolean } => {
  // 无父ID：处理顶级部门
  if (parentId === undefined) {
    const data = deptData.map(item => ({
      dept_id: item.dept_id,
      name: item.name,
      sort_num: item.sort_num
    }))
    return { data, visible: true }
  }

  // 有父ID：查找父部门并处理子部门
  const targetDept = findDeptById(deptData, parentId)
  const data = targetDept?.children
    ? targetDept.children.map(item => ({
        dept_id: item.dept_id,
        name: item.name,
        sort_num: item.sort_num
      }))
    : []

  return { data, visible: true }
}

/**
 * 排序子部门按钮
 */
const sortSubDeptBtn = (parentId?: number) => {
  const { data, visible } = getSortSubDeptData(parentId, deptTree.value)
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
  const deptIds = sortableData.value.map(item => {
    return item.dept_id
  })
  await sortDept(currentParentId.value, deptIds)

  ElMessage.success('排序成功')
  await getDeptData()
  sortDialogVisible.value = false
}

/**
 * 表格内更新部门按钮
 */
const updateDeptBtn = (row: Department) => {
  dialogType.value = DialogType.UPDATE
  department.dept_id = row.dept_id
  department.name = row.name
  department.parent_id = row.parent_id
  dialogVisible.value = true
}

/**
 * 表格内删除部门按钮
 */
const deleteDeptBtn = async (deptId: number) => {
  const deptToDelete = findOriginal(deptShow.value, deptId)
  if (deptToDelete === undefined) {
    ElMessage.warning('部门不存在，无法删除')
    return
  }
  if (deptToDelete.children && deptToDelete.children.length > 0) {
    ElMessage.warning('存在子部门，无法删除')
    return
  }
  ElMessageBox.confirm(`请确认是否要删除 ${deptToDelete.name} 部门`).then(
    async () => {
      await deleteDept(deptToDelete.dept_id)
      ElMessage.success('删除成功')
      await getDeptData()
      queryParams.name = ''
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
      <el-form-item label="部门名称">
        <el-input v-model="queryParams.name" placeholder="部门名称" clearable />
      </el-form-item>
      <el-form-item>
        <el-button
          type="primary"
          :icon="Search"
          size="small"
          @click="execDeptFilter"
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
        <el-button @click="newDepartmentBtn">新增部门</el-button>
        <el-button
          :disabled="deptTree.length <= 1"
          @click="sortSubDeptBtn(undefined)"
          >排序</el-button
        >
      </el-col>
    </el-row>
    <!-- 表格 -->
    <el-table :data="deptShow" row-key="dept_id" default-expand-all>
      <el-table-column prop="name" label="部门名称" min-width="120px" />
      <el-table-column fixed="right" label="操作">
        <template v-slot="{ row }">
          <el-button plain type="primary" @click="updateDeptBtn(row)"
            >更新</el-button
          >
          <el-button
            v-if="row.child_count > 0"
            plain
            type="primary"
            :disabled="row.child_count === 1"
            @click="sortSubDeptBtn(row.dept_id)"
            >子部门排序</el-button
          >
          <el-button
            v-else
            plain
            type="danger"
            @click="deleteDeptBtn(row.dept_id)"
            >删除</el-button
          >
        </template>
      </el-table-column>
    </el-table>
  </dg-card>
  <!-- 新增/更新 对话框 -->
  <el-dialog
    :title="dialogType ? '更新部门' : '新增部门'"
    v-model="dialogVisible"
    width="400px"
  >
    <el-form
      ref="departmentForm"
      :rules="rules"
      :model="department"
      label-width="80px"
    >
      <el-form-item
        v-if="dialogType === DialogType.ADD"
        label="所属部门"
        prop="parent_id"
      >
        <el-cascader
          v-model="department.parent_id"
          :options="deptTree"
          clearable
          placeholder="请选择父部门，可为空"
          :props="{
            checkStrictly: true,
            emitPath: false,
            value: 'dept_id',
            label: 'name',
            children: 'children'
          }"
        />
      </el-form-item>
      <el-form-item label="部门名称" prop="name">
        <el-input v-model="department.name" placeholder="请输入部门名称" />
      </el-form-item>
    </el-form>
    <template #footer>
      <el-button @click="cancel()">取 消</el-button>
      <el-button type="primary" @click="submit(departmentForm)"
        >提 交</el-button
      >
    </template>
  </el-dialog>
  <!-- 子部门排序对话框 -->
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
