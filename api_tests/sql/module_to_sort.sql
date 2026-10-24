TRUNCATE TABLE "public"."sys_module" RESTART IDENTITY;

-- 模块表数据
INSERT INTO public.sys_module VALUES (1, '模块-1', NULL, 0, NULL, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_module VALUES (2, '模块-1-1', NULL, 0, 1, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_module VALUES (3, '模块-1-2', NULL, 1, 1, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_module VALUES (4, '模块-1-3', NULL, 2, 1, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_module VALUES (5, '模块-2', NULL, 1, NULL, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);

SELECT setval('"public"."sys_module_module_id_seq"', 5, true);
