<script setup lang="ts">
import { getUserList } from '@/api/user'
import { Search, Refresh } from '@element-plus/icons-vue'
import { Pagination } from '@/types/common'
import type { User, UserQueryParams } from '@/types/user'
import { onMounted, reactive, ref } from 'vue'
import { Department } from '@/types/department'
import { getDeptTree } from '@/api/department'
import { ElMessage } from 'element-plus/es'

const userList = ref<User[]>([])

/**
 * 查询表单参数
 */
const queryParams = reactive<UserQueryParams & Pagination>({
  page: 1,
  page_size: 10
})

/**
 * 总记录数
 */
const total = ref(0)

/**
 * 查询用户列表
 */
const handleQuery = async () => {
  const data = await getUserList(queryParams)
  userList.value = data.list
  queryParams.page = data.page
  queryParams.page_size = data.page_size
  total.value = data.total
}

/**
 * 重置查询表单
 */
const resetQuery = () => {
  queryParams.username = undefined
  queryParams.nickname = undefined
  queryParams.sex = undefined
  queryParams.dept_id = undefined
  queryParams.phone_number = undefined
  queryParams.email = undefined
  queryParams.status = undefined

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

const newUserBtn = () => {
  ElMessage.error('功能未实现')
}

const updateUserBtn = (user: User) => {
  ElMessage.error('功能未实现')
}

const setRoleBtn = (userId: number) => {
  ElMessage.error('功能未实现')
}

const deleteUserBtn = (userId: number) => {
  ElMessage.error('功能未实现')
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
      <el-form-item label="用户名">
        <el-input
          v-model="queryParams.username"
          placeholder="请输入用户名"
          clearable
        />
      </el-form-item>
      <el-form-item label="昵称">
        <el-input
          v-model="queryParams.nickname"
          placeholder="请输入昵称"
          clearable
        />
      </el-form-item>
      <el-form-item label="性别">
        <el-select
          v-model="queryParams.sex"
          placeholder="请选择性别"
          clearable
          style="width: 173px"
        >
          <el-option label="保密" :value="0" :key="0" />
          <el-option label="男" :value="1" :key="1" />
          <el-option label="女" :value="2" :key="2" />
        </el-select>
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
      <el-form-item label="手机号">
        <el-input
          v-model="queryParams.phone_number"
          placeholder="请输入手机号"
          clearable
        />
      </el-form-item>
      <el-form-item label="邮箱">
        <el-input
          v-model="queryParams.email"
          placeholder="请输入邮箱"
          clearable
        />
      </el-form-item>
      <el-form-item label="状态">
        <el-select
          v-model="queryParams.status"
          placeholder="请选择状态"
          clearable
          style="width: 173px"
        >
          <el-option label="正常" :value="0" />
          <el-option label="禁用" :value="1" />
        </el-select>
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
        <el-button @click="newUserBtn">新增用户</el-button>
      </el-col>
    </el-row>
    <!-- 表格 -->
    <el-table :data="userList" row-key="name" default-expand-all>
      <el-table-column prop="user_id" label="ID" min-width="120px" />
      <el-table-column prop="username" label="用户名" min-width="120px" />
      <el-table-column prop="nickname" label="昵称" min-width="120px" />
      <el-table-column prop="sex" label="性别" min-width="120px">
        <template v-slot="{ row }">
          <span v-if="row.sex === 0">保密</span>
          <span v-else-if="row.sex === 1">男</span>
          <span v-else-if="row.sex === 2">女</span>
        </template>
      </el-table-column>
      <el-table-column prop="status" label="状态" min-width="120px">
        <template v-slot="{ row }">
          <span v-if="row.status === 0">正常</span>
          <span v-else-if="row.status === 1">禁用</span>
        </template>
      </el-table-column>

      <el-table-column fixed="right" label="操作" min-width="256px">
        <template v-slot="{ row }">
          <el-button plain type="primary" @click="updateUserBtn(row)"
            >更新</el-button
          >
          <el-button plain type="warning" @click="setRoleBtn(row.user_id)"
            >分配角色</el-button
          >
          <el-button plain type="danger" @click="deleteUserBtn(row.user_id)"
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
  <!-- debug -->
  <!--
  queryParams: {{ queryParams }}<br />
  userList: {{ userList }}<br />
  total: {{ total }}
  -->
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
