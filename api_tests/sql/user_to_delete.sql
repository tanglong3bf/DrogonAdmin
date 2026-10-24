TRUNCATE TABLE "public"."sys_user_role", "public"."sys_role_dept", "public"."sys_user", "public"."sys_role", "public"."sys_dept" RESTART IDENTITY;

-- 部门数据
INSERT INTO "public"."sys_dept" VALUES (1, '技术部', 0, NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);

-- 角色部门关联数据
INSERT INTO "public"."sys_role_dept" VALUES (1, 1, 1, '2026-04-01 00:00:00');

-- 角色数据
INSERT INTO "public"."sys_role" VALUES (1, '角色-1', 'role-1', '技术部白名单', 0, NULL, 1, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);

-- 用户角色关联数据
INSERT INTO "public"."sys_user_role" VALUES (4, 1, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user" VALUES (1, 'admin123', '$2a$10$MoBFdqc0CWMsTWvOM3ugNevngom4g5Ss30uCr82HNrdLNuHk.M11G', '超级管理员', '#', 0, 1, NULL, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (2, 'user002', '$2a$10$PNUDum1n84pJTAbzXHIbzeNSE8tfTag3OieOvaJz/JT/.IgcXwdEW', '用户002', '#', 0, 1, NULL, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user" VALUES (3, 'user003', '$2a$10$pqi6byAMrBVVWh.DmuhwOO7zD4AlGYIfVkUWsmNIGByLBUmo4axS2', '用户003', '#', 1, 1, NULL, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (4, 'user004', '$2a$10$cUcFy4yufBAH1BxTPDIYjuMW1pC62p1KI1Ha7NSs8u84jTIig22oi', '用户004', '#', 1, 1, NULL, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (5, 'user005', '$2a$10$cUcFy4yufBAH1BxTPDIYjuMW1pC62p1KI1Ha7NSs8u84jTIig22oi', '用户005', '#', 0, 1, NULL, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);

SELECT setval('"public"."sys_dept_dept_id_seq"', 1, true);
SELECT setval('"public"."sys_role_role_id_seq"', 1, true);
SELECT setval('"public"."sys_user_user_id_seq"', 4, true);
