<script setup lang="ts">
import { ref, reactive } from 'vue'
import { useRouter } from 'vue-router'
import { ElMessage, type FormInstance, type FormRules } from 'element-plus'
import { User } from '@element-plus/icons-vue'
import { useCommonStore } from '@/stores/common'
import { useAuthStore } from '@/stores/auth'
import { login } from '@/api/auth'
import { LoginRequest, MenuResponse } from '@/types/auth'
import { SidebarMenu } from '@/types/menu'

const commonStore = useCommonStore()
const authStore = useAuthStore()
const router = useRouter()

/**
 * 登录表单的实体引用
 */
const formRef = ref<FormInstance>()

/**
 * 登录表单类型声明
 */
interface LoginForm {
  username: string
  password: string
  rememberMe: boolean
}

/**
 * 登录表单绑定的数据
 */
const formData = reactive<LoginForm>({
  username: 'admin123',
  password: '123456',
  rememberMe: true
})

/**
 * 参数校验
 */
const rules = reactive<FormRules<LoginForm>>({
  username: [
    { required: true, message: '请输入用户名', trigger: 'blur' },
    { min: 6, max: 20, message: '用户名长度6-20位', trigger: 'blur' }
  ],
  password: [
    { required: true, message: '请输入密码', trigger: 'blur' },
    { min: 6, max: 20, message: '密码长度6-20位', trigger: 'blur' }
  ]
})

/**
 * 请求登录
 * @param formEl 登录表单实例
 */
const onLogin = async (formEl?: FormInstance) => {
  const isValid = formEl?.validate()
  if (isValid) {
    // TODO: rememberMe
    const request: LoginRequest = {
      username: formData.username,
      password: formData.password
    }
    const result = await login(request)

    const toSidebarMenu = (menu_list: MenuResponse[]): SidebarMenu[] => {
      return menu_list.map((menu: MenuResponse): SidebarMenu => {
        const sidebarMenuBase: SidebarMenu = {
          index: menu.menu_id.toString(),
          name: menu.name,
          icon: menu.icon,
          component: menu.component
        }
        const children =
          menu.children && menu.children.length > 0
            ? toSidebarMenu(menu.children)
            : undefined
        const sidebarMenu: SidebarMenu =
          menu.type === 'menu'
            ? {
                ...sidebarMenuBase,
                children
              }
            : {
                ...sidebarMenuBase,
                path: menu.path
              }
        return sidebarMenu
      })
    }

    authStore.setToken(result.token)
    commonStore.setMenuList(toSidebarMenu(result.menu_list))
    ElMessage({ message: '登录成功', type: 'success' })
    router.push('/home')
  }
}
</script>

<template>
  <el-container>
    <el-main>
      <div id="wrapper">
        <h3>欢迎登录</h3>
        <el-text tag="p">请输入您的用户名和密码</el-text>
        <el-form :model="formData" ref="formRef" :rules="rules">
          <el-form-item prop="username">
            <el-input
              v-model="formData.username"
              placeholder="请输入用户名"
              size="large"
              :prefix-icon="User"
            />
          </el-form-item>
          <el-form-item prop="password">
            <el-input
              v-model="formData.password"
              placeholder="请输入密码"
              size="large"
              :prefix-icon="User"
              type="password"
              show-password
            />
          </el-form-item>
          <el-form-item prop="rememberMe">
            <el-checkbox
              v-model="formData.rememberMe"
              label="记住密码"
              size="large"
            />
          </el-form-item>
          <el-form-item>
            <el-button
              type="primary"
              size="large"
              id="loginBtn"
              @click="onLogin(formRef)"
            >
              登录
            </el-button>
          </el-form-item>
        </el-form>
      </div>
    </el-main>
  </el-container>
</template>

<style scoped lang="scss">
#wrapper {
  width: 400px;
  padding-top: 100px;
  margin: 0 auto;
  h3 {
    font-size: 34px;
  }
  .el-text {
    margin-top: 10px;
  }
  .el-form {
    margin-top: 25px;
    #loginBtn {
      width: 100%;
    }
  }
}
</style>
