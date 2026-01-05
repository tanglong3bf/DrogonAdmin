import './assets/common.scss'
import 'element-plus/dist/index.css'
import { createApp } from 'vue'
import { createPinia } from 'pinia'
import piniaPluginPersistedstate from 'pinia-plugin-persistedstate'

import App from './App.vue'
import router from './router'

// 引入element-plus的图标
import * as ElementPlusIconsVue from '@element-plus/icons-vue'

import '@/mock'

const app = createApp(App)

// 注册所有的element-plus的图标
for (const [key, component] of Object.entries(ElementPlusIconsVue)) {
  app.component(key, component)
}

// pinia
const pinia = createPinia()
// pinia持久化存储
pinia.use(piniaPluginPersistedstate)
app.use(pinia)

// 路由
app.use(router)

app.mount('#app')
