import { defineStore } from 'pinia'
import { ref } from 'vue'

export const useAuthStore = defineStore(
  'auth',
  () => {
    const token = ref<string | undefined>(undefined)

    const setToken = (newToken: string | undefined) => {
      token.value = newToken
    }

    return {
      token,
      setToken
    }
  },
  {
    persist: true
  }
)
