<script setup lang="ts">
import { ref, reactive, onMounted } from 'vue'
import { useRouter } from 'vue-router'
import { ElMessage, type FormInstance, type FormRules } from 'element-plus'
import { User } from '@element-plus/icons-vue'
import { useCommonStore } from '@/stores/common'
import { useAuthStore } from '@/stores/auth'
import { login } from '@/api/auth'
import { LoginFormData, LoginRequest, MenuResponse } from '@/types/auth'
import { SidebarMenu } from '@/types/menu'
import { MenuType } from '@/types/enums'
import { API_BASE_URL } from '@/config'
import axios, { AxiosError } from 'axios'

const commonStore = useCommonStore()
const authStore = useAuthStore()
const router = useRouter()

/**
 * 登录表单的实体引用
 */
const formRef = ref<FormInstance>()

/**
 * 登录表单绑定的数据
 */
const formData = reactive<LoginFormData>({
  username: 'admin123',
  password: '123456',
  captchaId: '',
  captcha: '',
  rememberMe: true
})

/**
 * 验证码图片地址
 */
const captchaSrc = ref('')

onMounted(async () => {
  await refreshCaptcha()
})

const refreshCaptcha = async () => {
  if (captchaSrc.value.startsWith('blob:')) {
    URL.revokeObjectURL(captchaSrc.value)
  }
  try {
    const res = await axios.get(`${API_BASE_URL}/captcha`, {
      params: {
        // 后端以此为依据删除无效的redis，同时避免浏览器缓存
        old_captcha_id: formData.captchaId ? formData.captchaId : undefined,
        t: formData.captchaId ? undefined : Date.now()
      },
      responseType: 'blob'
    })
    formData.captchaId = res.headers['x-captcha-id']

    const blob = res.data
    captchaSrc.value = URL.createObjectURL(blob)
  } catch (err) {
    if ((err as AxiosError).status === 429) {
      ElMessage.error('系统繁忙，请稍后再试')
      return
    }
    console.error('获取验证码失败', err)
  }
}

/**
 * 参数校验
 */
const rules = reactive<FormRules<LoginFormData>>({
  username: [
    { required: true, message: '请输入用户名', trigger: 'blur' },
    { min: 6, max: 20, message: '用户名长度6-20位', trigger: 'blur' }
  ],
  password: [
    { required: true, message: '请输入密码', trigger: 'blur' },
    { min: 6, max: 20, message: '密码长度6-20位', trigger: 'blur' }
  ],
  captcha: [
    { required: true, message: '请输入验证码', trigger: 'blur' },
    { min: 4, max: 4, message: '请输入正确的验证码', trigger: 'blur' }
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
      password: formData.password,
      captchaId: formData.captchaId,
      captcha: formData.captcha
    }
    try {
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
            menu.type === MenuType.Menu
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

      authStore.setUserInfo(result.user_info)
      authStore.setToken(result.token)
      commonStore.setMenuList(toSidebarMenu(result.menu_list))
      ElMessage({ message: '登录成功', type: 'success' })
      router.push('/home')
    } catch (err) {
      console.error(err)
      await refreshCaptcha()
    }
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
          <!-- 验证码表单项 -->
          <el-form-item prop="captcha">
            <div class="captcha-row">
              <el-input
                v-model="formData.captcha"
                placeholder="请输入验证码"
                size="large"
              />
              <img
                class="captcha-img"
                :src="captchaSrc"
                alt="验证码"
                @click="refreshCaptcha"
              />
            </div>
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
.captcha-row {
  display: flex;
  gap: 12px;
  align-items: center;

  :deep(.el-input) {
    flex: 1;
  }
}

.captcha-img {
  /*后端输出 160×60，前端缩小展示*/
  width: 130px;
  height: 48px;
  object-fit: contain;
  cursor: pointer;
  border-radius: 4px;
  border: 1px solid #dcdfe6;
}
</style>
