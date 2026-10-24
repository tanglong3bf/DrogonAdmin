TRUNCATE TABLE "public"."sys_module", "public"."sys_action", "public"."sys_action_priority" RESTART IDENTITY;

SELECT setval('"public"."sys_module_module_id_seq"', 1, false);
SELECT setval('"public"."sys_action_action_id_seq"', 1, false);
