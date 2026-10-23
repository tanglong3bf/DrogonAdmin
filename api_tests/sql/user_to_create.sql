TRUNCATE TABLE "public"."sys_user_role", "public"."sys_role_dept", "public"."sys_user", "public"."sys_role", "public"."sys_dept" RESTART IDENTITY;

-- 部门数据
INSERT INTO "public"."sys_dept" VALUES (1, '技术部', 0, NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (2, '产品部', 1, NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);

-- 角色部门关联数据
INSERT INTO "public"."sys_role_dept" VALUES (5, 1, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_role_dept" VALUES (6, 1, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_role_dept" VALUES (7, 1, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_role_dept" VALUES (7, 2, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_role_dept" VALUES (8, 1, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_role_dept" VALUES (9, 1, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_role_dept" VALUES (10, 1, 1, '2026-04-01 00:00:00');

-- 角色数据
INSERT INTO "public"."sys_role" VALUES (1, '角色-1', 'role-1', '随便写点', 0, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (2, '角色-2', 'role-2', '没人看吧', 0, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (3, '角色-3', 'role-3', '限制总数量为sin^2(x)+cos^2(x)', 1, 1, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (4, '角色-4', 'role-4', '限制每部门数量为-e^{i \pai}', 2, 1, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (5, '角色-5', 'role-5', '白名单-技术部', 0, NULL, 1, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (6, '角色-6', 'role-6', '白名单-限制总数量为i^4', 1, 1, 1, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (7, '角色-7', 'role-7', '白名单-限制每部门数量为\lim\limits_{x\to0}\frac{\sin x}{x}', 2, 1, 1, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (8, '角色-8', 'role-8', '黑名单-技术部', 0, NULL, 2, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (9, '角色-9', 'role-9', '黑名单-限制总数量为e^{i\theta}\cdot(\cos\theta - i\sin\theta)', 1, 1, 2, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (10, '角色-10', 'role-10', '黑名单-限制每部门数量为\csc^2 x - \cot^2 x', 2, 1, 2, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);

-- 用户角色关联数据
INSERT INTO "public"."sys_user_role" VALUES (1, 3, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (2, 4, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (3, 4, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (4, 6, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (5, 7, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (6, 7, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (7, 9, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (8, 10, 1, '2026-04-01 00:00:00');

-- 用户数据
INSERT INTO "public"."sys_user" VALUES (1, 'placeholder_user001', '$2a$10$ISBUI0ipyYlyeSHD.iibbu5JBS0scCbikMPLw42OLQpZfYxhh8ZhG', '占位用户001', '#', 0, 1, NULL, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (2, 'placeholder_user002', '$2a$10$5JBvHOdBRbxAAxuEJp1QG...1uJaO0QsVBBLSJBy7q7OvSTzo/xJu', '占位用户002', '#', 0, 1, NULL, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (3, 'placeholder_user003', '$2a$10$KSpTDcnV7xSrFh3rF.qLxuomt.0USFCk6/FXEXOeJilvWq3g8Yxp.', '占位用户003', '#', 0, 2, NULL, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (4, 'placeholder_user004', '$2a$10$dEbxTWwTVfX5tx8WEYenCeWGiwlvij4X4WmKIAn3O7u86EXzdU/c6', '占位用户004', '#', 0, 2, NULL, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (5, 'placeholder_user005', '$2a$10$HR8yXMnRZodw3XJSgqtRbeiI6Lu7JeG6XTKEgRddWOyTchhcn9pHi', '占位用户005', '#', 0, 1, NULL, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (6, 'placeholder_user006', '$2a$10$6KnTa7sk5gr/gHnbbKpGneziir0CxywgHv.m..c2gzxQS4N1DhUdS', '占位用户006', '#', 0, 2, NULL, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (7, 'placeholder_user007', '$2a$10$vpflLdjeT3g6iQ4cQ2DL6uSCmUcPOwOZut2E4CTFpwIzvfF0aP2I2', '占位用户007', '#', 0, 2, NULL, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (8, 'placeholder_user008', '$2a$10$K0ZOjFyAJIGiX72aXHVvh.1gQ7q3gSSxCxwdUXzFRbM1Qbt4uY1Ya', '占位用户008', '#', 0, 2, NULL, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);

SELECT setval('"public"."sys_dept_dept_id_seq"', 2, true);
SELECT setval('"public"."sys_role_role_id_seq"', 10, true);
SELECT setval('"public"."sys_user_user_id_seq"', 8, true);
