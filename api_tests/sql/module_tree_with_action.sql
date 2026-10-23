TRUNCATE TABLE "public"."sys_module", "public"."sys_action", "public"."sys_action_priority" RESTART IDENTITY;

-- 模块表数据
INSERT INTO public.sys_module VALUES (1, '系统管理', null, 0, null, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (2, '组织架构', null, 0, 1, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (3, '部门管理', null, 0, 2, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (4, '用户管理', null, 1, 2, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (5, '权限管理', null, 1, 1, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (6, '角色管理', null, 0, 5, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (7, '权限分配', null, 1, 5, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);

-- 功能表数据
INSERT INTO public.sys_action VALUES (1, '浏览部门管理页面', 'dept:view', null, 0, false, 3, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (2, '查询部门树', 'dept:query', null, 1, true, 3, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (3, '新增部门', 'dept:create', null, 2, true, 3, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (4, '排序部门', 'dept:sort', null, 3, true, 3, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (5, '更新部门', 'dept:update', null, 4, true, 3, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (6, '删除部门', 'dept:delete', null, 5, true, 3, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (7, '浏览用户管理页面', 'user:view', null, 0, false, 4, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (8, '新增用户', 'user:create', null, 1, true, 4, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (9, '查询用户', 'user:query', null, 2, true, 4, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (10, '更新用户', 'user:update', null, 3, true, 4, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (11, '删除用户', 'user:delete', null, 4, true, 4, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (12, '浏览角色管理页面', 'role:view', null, 0, false, 6, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (13, '新增角色', 'role:create', null, 1, false, 6, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (14, '查询角色', 'role:query', null, 2, false, 6, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (15, '更新角色', 'role:update', null, 3, false, 6, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (16, '删除角色', 'role:delete', null, 4, false, 6, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);

--- 优先级表数据
INSERT INTO public.sys_action_priority VALUES (2, 3, 3, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (2, 4, 3, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (2, 5, 3, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (5, 6, 3, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (7, 8, 4, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (8, 9, 4, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (8, 10, 4, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (10, 11, 4, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (12, 13, 6, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (13, 14, 6, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (13, 15, 6, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (15, 16, 6, 1, '2026-07-22 15:32:58.000000');

SELECT setval('"public"."sys_module_module_id_seq"', 7, true);
SELECT setval('"public"."sys_action_action_id_seq"', 16, true);
