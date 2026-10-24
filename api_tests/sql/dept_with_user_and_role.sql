TRUNCATE TABLE "public"."sys_dept", "public"."sys_user", "public"."sys_role_dept", "public"."sys_role" RESTART IDENTITY;

INSERT INTO "public"."sys_dept" VALUES (1, '钱途无量有限公司', 0, NULL, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (2, '人事部', 0, 1, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (3, '财务部', 1, 1, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (4, '技术部', 2, 1, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (5, '前端组', 0, 4, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (6, '后端组', 1, 4, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (7, '测试组', 2, 4, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (8, '永不有限加班公司', 1, NULL, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (9, '人事部', 0, 8, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (10, '另一个分公司', 0, NULL, 1, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);

INSERT INTO "public"."sys_user" VALUES (1, 'gugugaga', '$2a$10$yniicB8J5B9lvCWuHYP23eXxztTI5pIjpM1rD2m2S.YvFw6zqiXiS', '咕咕嘎嘎', '#', 1, 6, NULL, NULL, 0, 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);

INSERT INTO "public"."sys_role_dept" VALUES (1, 5, 1, '2026-04-03 22:28:44');
INSERT INTO "public"."sys_role_dept" VALUES (2, 7, 1, '2026-04-03 22:28:44');

INSERT INTO "public"."sys_role" VALUES (1, '起个啥名好呢', 'what is your name', '随便写写，没人看吧', 0, NULL, 1, 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (2, '不能重名', 'my name is li hua', '随便写写，没人看吧', 0, NULL, 2, 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);

SELECT setval('"public"."sys_dept_dept_id_seq"', 9, true);
SELECT setval('"public"."sys_user_user_id_seq"', 1, true);
SELECT setval('"public"."sys_role_role_id_seq"', 1, false);
