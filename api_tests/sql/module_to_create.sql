TRUNCATE TABLE "public"."sys_module" RESTART IDENTITY;

-- 模块表数据
INSERT INTO public.sys_module VALUES (1, '占位模块-1', NULL, 0, NULL, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_module VALUES (2, '占位模块-2', NULL, 1, NULL, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);

SELECT setval('"public"."sys_module_module_id_seq"', 2, true);
