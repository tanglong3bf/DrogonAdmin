<script lang="ts" setup>
import dgCard from '@/components/dg-card.vue'
import { Department } from '@/types/department'
import { Search, Refresh } from '@element-plus/icons-vue'
import { FormInstance } from 'element-plus/es'
import { onMounted, reactive, ref } from 'vue'
import { getDeptTree } from '@/api/department'
import { getRoleList } from '@/api/role'
import type { Role, RoleQueryParams } from '@/types/role'

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
const queryParams = reactive<RoleQueryParams>({
  name: '',
  dept_id: undefined,
  page: 1,
  page_size: 10
})

/**
 * 查询角色列表
 */
const handleQuery = async () => {
  const data = await getRoleList(queryParams)
  roleList.value = data.list
  queryParams.page = data.page
  queryParams.page_size = data.page_size
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

const newRoleBtn = () => {
  console.log('新增角色')
}

const updateRoleBtn = (row: Role) => {
  console.log('更新角色', row)
}

const deleteRoleBtn = (role_id: number) => {
  console.log('删除角色', role_id)
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
      <el-table-column prop="description" label="角色描述" min-width="120px" />
      <el-table-column prop="user_quota" label="限制数量" min-width="120px" />
      <el-table-column prop="quota_type" label="限制类型" min-width="120px" />
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
</style>
