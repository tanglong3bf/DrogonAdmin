<script lang="ts" setup>
import BasicInfo from './component/BasicInfo.vue'
import BasicSetting from './component/BasicSetting.vue'
import PasswordSetting from './component/PasswordSetting.vue'
</script>

<template>
  <el-row :gutter="20">
    <el-col :span="8">
      <basic-info />
    </el-col>
    <el-col :span="16">
      <el-card style="border-radius: 12px">
        <el-tabs type="card">
          <el-tab-pane label="基本设置">
            <basic-setting />
          </el-tab-pane>
          <el-tab-pane label="修改密码">
            <password-setting />
          </el-tab-pane>
        </el-tabs>
      </el-card>
    </el-col>
  </el-row>
</template>
