import { UserInfo } from '@/types/auth'
import { defineStore } from 'pinia'
import { ref } from 'vue'

export const useAuthStore = defineStore(
  'auth',
  () => {
    /**
     * 令牌
     */
    const token = ref<string | undefined>(undefined)

    /**
     * 设置令牌
     */
    const setToken = (newToken?: string) => {
      token.value = newToken
    }

    /**
     * 用户信息
     */
    const userInfo = ref<UserInfo | undefined>(undefined)

    /**
     * 设置用户信息
     */
    const setUserInfo = (newUserInfo?: UserInfo) => {
      userInfo.value = newUserInfo
    }

    return {
      token,
      setToken,
      userInfo,
      setUserInfo
    }
  },
  {
    persist: true
  }
)
