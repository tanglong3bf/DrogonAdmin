TRUNCATE TABLE "public"."sys_dept" RESTART IDENTITY;

SELECT setval('"public"."sys_dept_dept_id_seq"', 1, false);
