TRUNCATE TABLE "public"."sys_role", "public"."sys_permission", "public"."sys_module", "public"."sys_action", "public"."sys_menu" RESTART IDENTITY;

-- 角色表数据
INSERT INTO public.sys_role VALUES (1, '超级管理员', 'admin', '分配了user:*权限', 0, NULL, 0, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);

-- 权限表数据
INSERT INTO public.sys_permission VALUES (1, 3, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_permission VALUES (1, 4, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);

-- 模块表数据
INSERT INTO public.sys_module VALUES (1, '模块-1', '已经被软删除的模块', 0, NULL, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000');
INSERT INTO public.sys_module VALUES (2, '模块-2', '有子模块的模块', 1, NULL, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_module VALUES (3, '模块-3', '占位子模块', 0, 2, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_module VALUES (4, '模块-4', '有功能的模块', 2, NULL, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_module VALUES (5, '模块-5', '功能被分配了权限', 3, NULL, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_module VALUES (6, '模块-6', '功能被菜单使用', 3, NULL, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);

-- 功能表数据
INSERT INTO public.sys_action VALUES (1, '浏览部门管理页面', 'dept:view', NULL, 0, false, 4, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_action VALUES (2, '新增部门', 'dept:create', NULL, 1, false, 4, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_action VALUES (3, '浏览用户管理页面', 'user:view', NULL, 0, false, 5, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_action VALUES (4, '新增用户', 'user:create', NULL, 1, false, 5, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_action VALUES (5, '浏览角色管理页面', 'role:view', '被菜单使用', 0, false, 6, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);

-- 菜单表数据
INSERT INTO public.sys_menu VALUES (1, 5, NULL, '/role', NULL, '角色管理', 0, '/path', 1, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);

SELECT setval('"public"."sys_role_role_id_seq"', 1, true);
SELECT setval('"public"."sys_module_module_id_seq"', 6, true);
SELECT setval('"public"."sys_action_action_id_seq"', 5, true);
SELECT setval('"public"."sys_menu_menu_id_seq"', 1, true);
