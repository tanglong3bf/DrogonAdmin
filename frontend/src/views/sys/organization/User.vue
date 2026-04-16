<script setup lang="ts">
import { deleteUser, getUserList, newUser, updateUser } from '@/api/user'
import { Search, Refresh } from '@element-plus/icons-vue'
import { Pagination } from '@/types/common'
import type { User, UserFormData, UserQueryParams } from '@/types/user'
import { onMounted, reactive, ref, watch } from 'vue'
import { Department } from '@/types/department'
import { getDeptTree } from '@/api/department'
import {
  ElMessage,
  ElMessageBox,
  FormInstance,
  FormRules
} from 'element-plus/es'
import { DialogType, Sex, Status } from '@/types/enums'
import { getAssignableRoles } from '@/api/role'
import { RoleOption } from '@/types/role'

/**
 * 用户列表
 */
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

const roleList = ref<RoleOption[]>([])

onMounted(async () => {
  deptTree.value = await getDeptTree()
  roleList.value = await getAssignableRoles(user.dept_id)
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

/**
 * 对话框类型
 */
const dialogType = ref<DialogType>(DialogType.ADD)

/**
 * 用户表单数据
 */
const user = reactive<UserFormData>({
  status: Status.Enable,
  sex: Sex.Secrecy
})

/**
 * 用户表单实例
 */
const userForm = ref<FormInstance>()

/**
 * 表单验证规则
 */
const rules = reactive<FormRules<UserFormData>>({
  username: [
    {
      required: true,
      message: '请输入用户名',
      trigger: 'blur'
    },
    {
      pattern: /^[a-zA-Z0-9_]+$/,
      message: '用户名只能包含字母、数字和下划线',
      trigger: 'blur'
    }
  ],
  nickname: [
    {
      required: true,
      message: '请输入昵称',
      trigger: 'blur'
    }
  ],
  sex: [
    {
      required: true,
      message: '请选择性别',
      trigger: 'change'
    }
  ],
  dept_id: [
    {
      required: true,
      message: '请选择部门',
      trigger: 'change'
    }
  ],
  phone_number: {
    pattern:
      /^1(?:3[0-9]|4[013456789]|5[0-35-9]|6[0-9]|7[0-8]|8[0-9]|9[0-9])\d{8}$/,
    message: '手机号格式错误',
    trigger: 'blur'
  },
  email: {
    pattern: /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/,
    message: '邮箱格式错误',
    trigger: 'blur'
  },
  status: [
    {
      required: true,
      message: '请选择状态',
      trigger: 'change'
    }
  ]
})

/**
 * 重置新增/更新用户表单
 */
const resetUserForm = () => {
  user.user_id = undefined
  user.username = undefined
  user.nickname = undefined
  user.sex = Sex.Secrecy
  user.dept_id = undefined
  user.phone_number = undefined
  user.email = undefined
  user.status = Status.Enable
  user.role_ids = undefined
}

/**
 * 显示新增/更新用户对话框
 */
const dialogVisible = ref(false)

/**
 * 监听部门id变化
 */
watch(
  () => user.dept_id, // getter 函数，监听 dept_id 的变化
  async (newVal, oldVal) => {
    if (newVal !== oldVal) {
      // 部门id发生切换时，重新获取可用角色
      const newRoles = await getAssignableRoles(newVal)
      roleList.value = newRoles

      // 清理 user.role_ids 中不在新 roleList 的项
      const validRoleIds = new Set(newRoles.map(r => r.role_id))
      user.role_ids = user.role_ids?.filter(id => validRoleIds.has(id))
    }
  },
  { immediate: true }
)

/**
 * 取消新增/更新
 */
const cancel = () => {
  dialogVisible.value = false
}

/**
 * 更新用户id，用于在更新后查找原始数据
 */
const userOriginalId = ref(0)

/**
 * 提交新增/更新用户表单
 */
const submit = (form?: FormInstance) => {
  form?.validate(async (valid: boolean, fields? /*:ValidateFieldsError*/) => {
    if (!valid) {
      // 弹出错误信息
      for (const key in fields) {
        ElMessage.error(fields[key][0].message)
      }
      return
    }
    // 新增
    if (dialogType.value === DialogType.ADD) {
      if (user.phone_number === '') {
        user.phone_number = undefined
      }
      if (user.email === '') {
        user.email = undefined
      }
      if (user.role_ids && user.role_ids.length === 0) {
        user.role_ids = undefined
      }
      await newUser(user)
      await handleQuery()
      dialogVisible.value = false
      resetUserForm()
    }
    // 更新
    else {
      console.log(user)
      const oldUser = userList.value.find(
        item => item.user_id === userOriginalId.value
      )
      console.log(oldUser)

      // 比较 user 与 oldUser
      const nicknameChanged = user.nickname !== oldUser?.nickname
      const sexChanged = user.sex !== oldUser?.sex
      const deptIdChanged = user.dept_id !== oldUser?.dept_id
      const phoneNumberChanged = user.phone_number !== oldUser?.phone_number
      const emailChanged = user.email !== oldUser?.email
      const statusChanged = user.status !== oldUser?.status

      // 构造参数
      const data: UserFormData = {
        nickname: nicknameChanged ? user.nickname : undefined,
        sex: sexChanged ? user.sex : undefined,
        dept_id: deptIdChanged ? user.dept_id : undefined,
        phone_number: phoneNumberChanged
          ? user.phone_number
            ? user.phone_number
            : null
          : undefined,
        email: emailChanged ? (user.email ? user.email : null) : undefined,
        status: statusChanged ? user.status : undefined,
        role_ids: user.role_ids
      }

      await updateUser(user.user_id!, data)
      await handleQuery()
      dialogVisible.value = false
    }
  })
}

/**
 * 新增用户
 */
const newUserBtn = () => {
  resetUserForm()
  dialogVisible.value = true
  dialogType.value = DialogType.ADD
}

/**
 * 更新用户按钮
 */
const updateUserBtn = (row: User) => {
  dialogType.value = DialogType.UPDATE
  userOriginalId.value = row.user_id
  user.user_id = row.user_id
  user.username = row.username
  user.nickname = row.nickname
  user.sex = row.sex
  user.dept_id = row.dept_id
  user.phone_number = row.phone_number
  user.email = row.email
  user.status = row.status
  user.role_ids = row.user_roles?.map(item => item.role_id)
  dialogVisible.value = true
}

const deleteUserBtn = async (userId: number) => {
  try {
    await ElMessageBox.confirm(
      `请确认是否要删除 ${userList.value.find(item => item.user_id === userId)?.username} 用户`,
      '提示',
      {
        confirmButtonText: '确定',
        cancelButtonText: '取消',
        type: 'warning'
      }
    )
    await deleteUser(userId)
    queryParams.page = 1
    resetQuery()
    await handleQuery()
  } catch (_ignore) {}
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
      <!-- 操作列 -->
      <el-table-column fixed="right" label="操作" min-width="256px">
        <template v-slot="{ row }">
          <el-button plain type="primary" @click="updateUserBtn(row)"
            >更新</el-button
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
  <!-- 新增/更新 对话框 -->
  <el-dialog
    :title="dialogType ? '更新用户' : '新增用户'"
    v-model="dialogVisible"
    width="650px"
  >
    <el-form ref="userForm" :rules="rules" :model="user" label-width="100px">
      <el-row :gutter="20">
        <el-col :span="12">
          <el-form-item label="用户名" prop="username">
            <el-input
              v-model="user.username"
              placeholder="请输入用户名"
              :disabled="dialogType === DialogType.UPDATE"
            />
          </el-form-item>
        </el-col>
        <el-col :span="12">
          <el-form-item label="昵称" prop="nickname">
            <el-input v-model="user.nickname" placeholder="请输入昵称" />
          </el-form-item>
        </el-col>
      </el-row>
      <!-- 来一个tag，提示默认密码为123456 -->

      <el-row :gutter="20">
        <el-col :span="12">
          <el-form-item label="性别" prop="sex">
            <el-radio-group v-model="user.sex">
              <el-radio :label="0">保密</el-radio>
              <el-radio :label="1">男</el-radio>
              <el-radio :label="2">女</el-radio>
            </el-radio-group>
          </el-form-item>
        </el-col>
        <el-col :span="12">
          <el-form-item label="所属部门" prop="dept_id">
            <el-cascader
              v-model="user.dept_id"
              :options="deptTree"
              placeholder="请选择部门"
              :props="{
                checkStrictly: true,
                emitPath: false,
                value: 'dept_id',
                label: 'name',
                children: 'children'
              }"
              filterable
              collapse-tags
            />
          </el-form-item>
        </el-col>
      </el-row>
      <el-row :gutter="20">
        <el-col :span="12">
          <el-form-item label="手机号" prop="phone_number">
            <el-input
              v-model="user.phone_number"
              placeholder="请输入手机号"
              clearable
            />
          </el-form-item>
        </el-col>
        <el-col :span="12">
          <el-form-item label="邮箱" prop="email">
            <el-input v-model="user.email" placeholder="请输入邮箱" clearable />
          </el-form-item>
        </el-col>
      </el-row>
      <el-row :gutter="20">
        <el-col :span="12">
          <el-form-item label="状态" prop="status">
            <el-radio-group v-model="user.status">
              <el-radio :label="0">正常</el-radio>
              <el-radio :label="1">禁用</el-radio>
            </el-radio-group>
          </el-form-item>
        </el-col>
        <el-col :span="12">
          <el-form-item label="角色" prop="role_ids">
            <el-select
              v-model="user.role_ids"
              multiple
              collapse-tags
              collapse-tags-tooltip
              placeholder="请选择角色"
              style="width: 240px"
            >
              <el-option
                v-for="item in roleList"
                :key="item.role_id"
                :label="item.name"
                :value="item.role_id"
              />
            </el-select>
          </el-form-item>
        </el-col>
      </el-row>
    </el-form>
    <template #footer>
      <el-button @click="cancel()">取 消</el-button>
      <el-button type="primary" @click="submit(userForm)">提 交</el-button>
    </template>
  </el-dialog>
  <!-- debug -->
  <!--
  dialogVisible: {{ dialogVisible }}<br />
  queryParams: {{ queryParams }}<br />
  userList: <br />
  <ul v-for="user in userList">
    <li>{{ user }}</li>
  </ul>
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
