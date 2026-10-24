TRUNCATE TABLE "public"."sys_module" RESTART IDENTITY;

INSERT INTO public.sys_module VALUES (1, '系统管理', null, 0, null, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (2, '组织架构', null, 0, 1, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (3, '部门管理', null, 0, 2, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (4, '用户管理', null, 1, 2, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (5, '权限管理', null, 1, 1, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (6, '角色管理', null, 0, 5, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (7, '权限分配', null, 1, 5, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);

SELECT setval('"public"."sys_module_module_id_seq"', 7, true);
