TRUNCATE TABLE "public"."sys_module", "public"."sys_action", "public"."sys_action_priority" RESTART IDENTITY;

-- 模块表数据
INSERT INTO public.sys_module VALUES (1, '模块-1', NULL, 0, NULL, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_module VALUES (2, '模块-2', NULL, 1, NULL, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_module VALUES (3, '模块-3', NULL, 2, NULL, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);

-- 功能表数据
INSERT INTO public.sys_action VALUES (1, '功能-1', 'action-1', null, 0, false, 1, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (2, '功能-2', 'action-2', null, 1, true, 1, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (3, '功能-3', 'action-3', null, 2, true, 1, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (4, '功能-4', 'action-4', null, 3, true, 1, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (5, '功能-5', 'action-5', null, 4, true, 1, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (6, '功能-3-1', 'action-3-1', null, 0, false, 3, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (7, '功能-3-2', 'action-3-2', null, 1, false, 3, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);

--- 优先级表数据
INSERT INTO public.sys_action_priority VALUES (2, 3, 1, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (2, 4, 1, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (2, 5, 1, 1, '2026-07-22 15:32:58.000000');

SELECT setval('"public"."sys_module_module_id_seq"', 3, true);
SELECT setval('"public"."sys_action_action_id_seq"', 7, true);
