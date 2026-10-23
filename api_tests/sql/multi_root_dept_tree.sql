TRUNCATE TABLE "public"."sys_dept" RESTART IDENTITY;

INSERT INTO "public"."sys_dept" VALUES (1, '钱途无量有限公司', 0, NULL, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (2, '人事部', 0, 1, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (3, '财务部', 1, 1, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (4, '技术部', 2, 1, 1, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (5, '前端组', 0, 4, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (6, '后端组', 1, 4, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (7, '测试组', 2, 4, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (8, '永不有限加班公司', 1, NULL, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (9, '人事部', 0, 8, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);

SELECT setval('"public"."sys_dept_dept_id_seq"', 9, true);
