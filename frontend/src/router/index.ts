import { createRouter, createWebHistory } from 'vue-router'

const routes = [
  {
    path: '/',
    component: () => import('@/layouts/AdminLayout.vue'),
    redirect: '/home',
    children: [
      {
        path: '/home',
        component: () => import('@/views/Home.vue')
      }
    ]
  }
]

const router = createRouter({
  history: createWebHistory(),
  routes
})
export default router
