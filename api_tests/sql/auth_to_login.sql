TRUNCATE TABLE "public"."sys_user", "public"."sys_dept" RESTART IDENTITY;

-- 部门数据
INSERT INTO "public"."sys_dept" VALUES (1, '技术部', 0, NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);

-- 用户数据
INSERT INTO "public"."sys_user" VALUES (1, 'admin123', '$2a$10$hdjImi9IKKR/mqlJ29x5t.P7yGE1b1GRMGrt5kBtXj.a.NTN5iDiK', '超级管理员', '#', 0, 1, NULL, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);

SELECT setval('"public"."sys_dept_dept_id_seq"', 1, true);
SELECT setval('"public"."sys_user_user_id_seq"', 1, true);
