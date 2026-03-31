<script lang="ts" setup>
import dgCard from '@/components/dg-card.vue'
import { Department } from '@/types/department'
import { Search, Refresh } from '@element-plus/icons-vue'
import { ElMessage, FormInstance, FormRules } from 'element-plus/es'
import { onMounted, reactive, ref } from 'vue'
import { getDeptTree } from '@/api/department'
import { getRoleList, newRole } from '@/api/role'
import {
  type Role,
  type RoleFormData,
  type RoleQueryParams
} from '@/types/role'
import { Pagination } from '@/types/common'

enum QuotaType {
  Unlimited = 0,
  TotalLimit = 1,
  PerDeptLimit = 2
}

enum RelationType {
  All = 0,
  Whitelist = 1,
  Blacklist = 2
}

/**
 * 从后端返回的真实数据
 */
const roleList = ref<Role[]>([])

/**
 * 查询表单
 */
const queryForm = ref<FormInstance>()

/**
 * 查询表单参数
 */
const queryParams = reactive<RoleQueryParams & Pagination>({
  name: '',
  dept_id: undefined,
  page: 1,
  page_size: 10
})

/**
 * 总记录数
 */
const total = ref(0)

/**
 * 查询角色列表
 */
const handleQuery = async () => {
  const data = await getRoleList(queryParams)
  roleList.value = data.list
  queryParams.page = data.page
  queryParams.page_size = data.page_size
  total.value = data.total
}

/**
 * 重置查询表单
 */
const resetQuery = () => {
  queryParams.name = ''
  queryParams.dept_id = undefined
  queryParams.page = 1
  queryParams.page_size = 10
}

/**
 * 用于过滤条件
 */
const deptTree = ref<Department[]>([])

onMounted(async () => {
  deptTree.value = await getDeptTree()
  resetQuery()
  handleQuery()
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
 * 角色表单数据
 */
const role = reactive<RoleFormData>({
  name: '',
  code: '',
  description: '',
  quota_type: QuotaType.Unlimited,
  user_quota: 0,
  relation_type: RelationType.All
})

/**
 * 角色表单实例
 */
const roleForm = ref<FormInstance>()

/**
 * 表单验证规则
 */
const rules = reactive<FormRules<RoleFormData>>({
  name: [{ required: true, message: '请输入角色名称', trigger: 'blur' }],
  code: [{ required: true, message: '请输入角色代码', trigger: 'blur' }],
  quota_type: [
    { required: true, message: '请选择限制类型', trigger: 'change' }
  ],
  user_quota: [
    {
      validator(_rule, value: number, callback) {
        if (role.quota_type === QuotaType.Unlimited) {
          callback()
        } else if (value && value > 0) {
          callback()
        } else {
          callback(new Error('请输入数量限制'))
        }
      }
    },
    { type: 'number', message: '数量限制必须为数字', trigger: 'blur' }
  ],
  relation_type: [
    { required: true, message: '请选择生效部门', trigger: 'change' }
  ],
  dept_ids: [
    {
      validator(_rule, value: number[], callback) {
        if (role.relation_type === RelationType.All) {
          callback()
        } else if (value && value.length > 0) {
          callback()
        } else {
          callback(new Error('请选择生效部门'))
        }
      }
    }
  ]
})

/**
 * 重置新增\更新角色表单
 */
const resetRoleForm = () => {
  role.name = ''
  role.code = ''
  role.description = ''
  role.quota_type = QuotaType.Unlimited
}

/**
 * 新增角色按钮
 */
const newRoleBtn = () => {
  resetRoleForm()

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
 * 提交新增\更新角色表单
 */
const submit = async (form?: FormInstance) => {
  const isValid = await form?.validate()
  if (!isValid) {
    return
  }
  await newRole(role)
  await handleQuery()
  dialogVisible.value = false
  resetRoleForm()
}

/**
 * 修改分页器每页显示条数
 */
const handleSizeChange = (size: number) => {
  queryParams.page_size = size
  handleQuery()
}

/**
 * 修改分页器当前页
 */
const handleCurrentChange = (page: number) => {
  queryParams.page = page
  handleQuery()
}

const updateRoleBtn = (row: Role) => {
  console.log('更新角色', row)
  ElMessage.error('还没做噢')
}

const deleteRoleBtn = (role_id: number) => {
  console.log('删除角色', role_id)
  ElMessage.error('还没做噢')
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
      <el-form-item label="角色名字">
        <el-input
          v-model="queryParams.name"
          placeholder="请输入角色名字"
          clearable
        />
      </el-form-item>
      <el-form-item label="所属部门">
        <el-cascader
          v-model="queryParams.dept_id"
          :options="deptTree"
          placeholder="请选择部门"
          clearable
          :props="{
            checkStrictly: true,
            emitPath: false,
            value: 'dept_id',
            label: 'name',
            children: 'children'
          }"
        />
      </el-form-item>
      <el-form-item>
        <el-button
          type="primary"
          :icon="Search"
          size="small"
          @click="handleQuery"
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
        <el-button @click="newRoleBtn">新增角色</el-button>
      </el-col>
    </el-row>
    <!-- 表格 -->
    <el-table :data="roleList" row-key="name" default-expand-all>
      <el-table-column prop="name" label="角色名称" min-width="120px" />
      <el-table-column prop="code" label="角色代码" min-width="120px" />
      <el-table-column prop="description" label="角色描述" min-width="120px">
        <template v-slot="{ row }">
          <span
            v-if="row.description === undefined"
            style="color: #909399; font-size: 12px; margin-left: 5px"
          >
            (Null)
          </span>
          <span v-else :title="row.description"> {{ row.description }}</span>
        </template>
      </el-table-column>
      <el-table-column prop="quota_type" label="限制数量" min-width="120px">
        <template v-slot="{ row }">
          <span
            v-if="row.quota_type === QuotaType.Unlimited"
            class="el-tag el-tag--info"
          >
            不限制
          </span>
          <span
            v-if="row.quota_type === QuotaType.TotalLimit"
            class="el-tag el-tag--success"
          >
            {{ row.user_quota }}个
          </span>
          <span
            v-if="row.quota_type === QuotaType.PerDeptLimit"
            class="el-tag el-tag--warning"
          >
            {{ row.user_quota }}个/每部门
          </span>
        </template>
      </el-table-column>
      <el-table-column fixed="right" label="操作">
        <template v-slot="{ row }">
          <el-button plain type="primary" @click="updateRoleBtn(row)"
            >更新</el-button
          >
          <el-button plain type="danger" @click="deleteRoleBtn(row.role_id)"
            >删除</el-button
          >
        </template>
      </el-table-column>
    </el-table>
  </dg-card>
  <!-- 分页器 -->
  <el-pagination
    class="pagination"
    background
    :hide-on-single-page="false"
    layout="total, sizes, prev, pager, next, jumper"
    :total="total"
    @size-change="handleSizeChange"
    @current-change="handleCurrentChange"
  />
  <!-- 新增/更新 对话框 -->
  <el-dialog
    :title="dialogType ? '更新角色' : '新增角色'"
    v-model="dialogVisible"
    width="650px"
  >
    <el-form ref="roleForm" :rules="rules" :model="role" label-width="100px">
      <el-row :gutter="20">
        <el-col :span="12">
          <el-form-item label="角色名称" prop="name">
            <el-input
              v-model="role.name"
              placeholder="请输入角色名称"
              clearable
            />
          </el-form-item>
        </el-col>
        <el-col :span="12">
          <el-form-item label="角色代码" prop="code">
            <el-input
              v-model="role.code"
              placeholder="请输入角色代码"
              clearable
            />
          </el-form-item>
        </el-col>
      </el-row>
      <el-form-item label="角色描述" prop="description">
        <el-input
          v-model="role.description"
          placeholder="请输入角色描述"
          clearable
        />
      </el-form-item>
      <el-form-item label="限制类型" prop="quota_type">
        <el-radio-group v-model="role.quota_type">
          <el-radio :label="QuotaType.Unlimited">不限制</el-radio>
          <el-radio :label="QuotaType.TotalLimit">总量限制</el-radio>
          <el-radio :label="QuotaType.PerDeptLimit">部门限制</el-radio>
        </el-radio-group>
      </el-form-item>
      <el-form-item
        label="限制数量"
        prop="user_quota"
        v-if="role.quota_type !== QuotaType.Unlimited"
      >
        <el-input-number v-model="role.user_quota" />
      </el-form-item>
      <el-form-item label="生效部门" prop="relation_type">
        <el-radio-group v-model="role.relation_type">
          <el-radio :label="RelationType.All">全部部门</el-radio>
          <el-radio :label="RelationType.Whitelist"
            >仅以下部门（白名单）</el-radio
          >
          <el-radio :label="RelationType.Blacklist"
            >除以下部门外（黑名单）</el-radio
          >
        </el-radio-group>
      </el-form-item>

      <el-form-item
        v-if="role.relation_type !== RelationType.All"
        label=""
        prop="dept_ids"
      >
        <el-cascader
          v-model="role.dept_ids"
          :options="deptTree"
          placeholder="请选择部门（可多选）"
          clearable
          :props="{
            checkStrictly: true,
            emitPath: false,
            value: 'dept_id',
            label: 'name',
            children: 'children',
            multiple: true
          }"
          filterable
          collapse-tags
        />
      </el-form-item>
    </el-form>
    <template #footer>
      <el-button @click="cancel()">取 消</el-button>
      <el-button type="primary" @click="submit(roleForm)">提 交</el-button>
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

.pagination {
  margin-top: 15px;
  display: flex;
  justify-content: flex-end;
}
</style>
