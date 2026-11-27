import { addCollection } from '@iconify/vue'
import type { IconifyJSON } from '@iconify/types'

import fa6Solid from '@iconify-json/fa6-solid/icons.json'
import ep from '@iconify-json/ep/icons.json'
import fluentMdl2 from '@iconify-json/fluent-mdl2/icons.json'
import ion from '@iconify-json/ion/icons.json'
import ix from '@iconify-json/ix/icons.json'
import mdi from '@iconify-json/mdi/icons.json'
import mingcute from '@iconify-json/mingcute/icons.json'
import octicon from '@iconify-json/octicon/icons.json'
import ph from '@iconify-json/ph/icons.json'
import prime from '@iconify-json/prime/icons.json'
import ri from '@iconify-json/ri/icons.json'
import tdesign from '@iconify-json/tdesign/icons.json'

/**
 * 注册本地图标集合。
 *
 * @iconify/vue 的 Icon 组件默认对未命中的图标发起远程请求（api.iconify.design），
 * 菜单图标来自后端数据、名称在运行时才确定，无法被 vite-plugin-purge-icons 静态提取，
 * 因此必须显式注册集合，让这些前缀从本地解析。
 */
const collections: IconifyJSON[] = [
  fa6Solid as IconifyJSON,
  ep as IconifyJSON,
  fluentMdl2 as IconifyJSON,
  ion as IconifyJSON,
  ix as IconifyJSON,
  mdi as IconifyJSON,
  mingcute as IconifyJSON,
  octicon as IconifyJSON,
  ph as IconifyJSON,
  prime as IconifyJSON,
  ri as IconifyJSON,
  tdesign as IconifyJSON
]

for (const collection of collections) {
  addCollection(collection)
}
