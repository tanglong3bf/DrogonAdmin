TRUNCATE TABLE "public"."sys_module" RESTART IDENTITY;

INSERT INTO public.sys_module VALUES (1, '模块A', null, 0, null, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (2, '模块B', null, 1, null, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (3, '模块C', null, 2, null, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (4, '模块A-1', null, 0, 1, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (5, '模块A-2', null, 1, 1, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (6, '模块B-1', null, 0, 2, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (7, '模块C-1', null, 0, 3, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);

SELECT setval('"public"."sys_module_module_id_seq"', 7, true);
