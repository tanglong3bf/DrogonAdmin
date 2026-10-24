TRUNCATE TABLE "public"."sys_dept", "public"."sys_role", "public"."sys_role_dept" RESTART IDENTITY;

INSERT INTO "public"."sys_dept" VALUES (1, '部门-1', 0, NULL, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (2, '部门-2', 0, NULL, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (3, '部门-3', 0, NULL, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (4, '部门-4', 0, NULL, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (5, '部门-5', 0, NULL, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30');

INSERT INTO "public"."sys_role" VALUES (1, '管理员-1', 'admin-1', NULL, 0, NULL, 0, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (2, '管理员-2', 'admin-2', NULL, 0, NULL, 1, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (3, '管理员-3', 'admin-3', NULL, 0, NULL, 2, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);

INSERT INTO "public"."sys_role_dept" VALUES (2, 2, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (2, 4, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (3, 3, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (3, 4, 1, '2026-04-02 21:53:40');

SELECT setval('"public"."sys_dept_dept_id_seq"', 5, true);
SELECT setval('"public"."sys_role_role_id_seq"', 3, true);
