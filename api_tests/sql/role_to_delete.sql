TRUNCATE TABLE "public"."sys_dept", "public"."sys_role", "public"."sys_role_dept", "public"."sys_user_role", "public"."sys_user" RESTART IDENTITY;

INSERT INTO "public"."sys_dept" VALUES (1, '部门-1', 0, NULL, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);

INSERT INTO "public"."sys_role" VALUES (1, '管理员-1', 'admin-1', NULL, 0, NULL, 0, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (2, '管理员-2', 'admin-2', NULL, 0, NULL, 0, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (3, '管理员-3', 'admin-3', NULL, 0, NULL, 1, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (4, '管理员-4', 'admin-4', NULL, 0, NULL, 1, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);

INSERT INTO "public"."sys_role_dept" VALUES (3, 1, 1, '2026-04-02 21:53:40');

INSERT INTO "public"."sys_user_role" VALUES (1, 2, 1, '2026-04-02 21:53:40');

INSERT INTO "public"."sys_user" VALUES (1, 'user-1', '$2a$10$EdjMtPJT7oMfhrs1AI8MO.IncwJghXzfp4L7eEZjGotZXhxrx9g9O', '用户-1', '#', 1, 1, NULL, NULL, 0, 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);

SELECT setval('"public"."sys_dept_dept_id_seq"', 1, true);
SELECT setval('"public"."sys_role_role_id_seq"', 4, true);
SELECT setval('"public"."sys_user_user_id_seq"', 1, true);
