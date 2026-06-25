DROP TABLE IF EXISTS "public"."sys_user";
DROP SEQUENCE IF EXISTS "public"."sys_user_user_id_seq";
CREATE SEQUENCE "public"."sys_user_user_id_seq" 
INCREMENT 1
MINVALUE  1
MAXVALUE 2147483647
START 1
CACHE 1;
CREATE TABLE "public"."sys_user" (
  "user_id" int4 NOT NULL DEFAULT nextval('sys_user_user_id_seq'::regclass),
  "username" varchar(20) COLLATE "pg_catalog"."default" NOT NULL,
  "password" char(60) COLLATE "pg_catalog"."default" NOT NULL,
  "nickname" varchar(50) COLLATE "pg_catalog"."default" NOT NULL,
  "avatar" varchar(255) COLLATE "pg_catalog"."default" NOT NULL,
  "sex" int2 NOT NULL DEFAULT 0,
  "dept_id" int4 NOT NULL,
  "phone_number" char(11) COLLATE "pg_catalog"."default",
  "email" varchar(50) COLLATE "pg_catalog"."default",
  "status" int2 NOT NULL DEFAULT 0,
  "created_by" int4 NOT NULL,
  "created_time" timestamp(6) NOT NULL,
  "updated_by" int4 NOT NULL,
  "updated_time" timestamp(6) NOT NULL,
  "deleted_by" int4,
  "deleted_time" timestamp(6)
)
;
COMMENT ON COLUMN "public"."sys_user"."username" IS '用户名';
COMMENT ON COLUMN "public"."sys_user"."password" IS '密码';
COMMENT ON COLUMN "public"."sys_user"."nickname" IS '昵称';
COMMENT ON COLUMN "public"."sys_user"."avatar" IS '头像';
COMMENT ON COLUMN "public"."sys_user"."sex" IS '性别 0-保密 1-男 2-女';
COMMENT ON COLUMN "public"."sys_user"."dept_id" IS '所属部门id';
COMMENT ON COLUMN "public"."sys_user"."phone_number" IS '电话号码';
COMMENT ON COLUMN "public"."sys_user"."email" IS '邮箱';
COMMENT ON COLUMN "public"."sys_user"."status" IS '状态 0-正常 1-禁用';
COMMENT ON COLUMN "public"."sys_user"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_user"."created_time" IS '创建时间';
COMMENT ON COLUMN "public"."sys_user"."updated_by" IS '最后一次更新者';
COMMENT ON COLUMN "public"."sys_user"."updated_time" IS '最后一次更新时间';
COMMENT ON COLUMN "public"."sys_user"."deleted_by" IS '删除者';
COMMENT ON COLUMN "public"."sys_user"."deleted_time" IS '删除时间';

INSERT INTO "public"."sys_user" VALUES (1, 'admin-1', '$2a$10$kqq0NdN38dlbni.wyabBie6S3.VLcFAcEtFwOjpGQwGyMNHv2v0wS', '管理员-1', '#', 1, 1, '18888888888', 'hello@world.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (2, 'admin-2', '$2a$10$ShQJGshUe0nJkxMBrpD.5ukEUNhkhQR0h2PoRCx.hU8xhMiMJyY6a', '管理员-2', '#', 2, 1, '13666666666', 'wanan@makabaka.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (3, '123456', '$2a$10$bwTUPnrIxotUsfk9kx607.9as5uAXzfAFxcZBT/Gvq5X00M1tCUNC', '超级管理员', '#', 1, 1, '13900000001', 'super@admin.com', 0, 1, '2026-04-04 08:00:00', 1, '2026-04-04 08:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (4, 'admin123', '$2a$10$P.lj9CubCBdaWrf.u1oW2...0DxxwCm1.4c43tsNII8XtnNR5QFXi', '小唐经理', '#', 2, 2, '13800000011', 'tang.manager@dept2.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (5, 'user_001', '$2a$10$tIGloD5jGcRTL5Bwt0ya8.9/Aj891B/v6Oz5AvPwHSQlQu.1zYRry', '张开发-张三', '#', 1, 3, '13700011100', 'zhang.dev@dept3.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (6, 'user_002', '$2a$10$0iHXZKR.AWJf4GVVRRuuuO2OW5rf4Ij17HGLZZ4DT2X73LP9aV2Fe', '张测试-李四', '#', 0, 3, '13700022200', 'zhang.qa@dept3.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (7, 'user_003', '$2a$10$p.fOW7voPUhZU2tUcwmxlu9oq1UMkBzjrnjToXX2iI7dwzgY5LyVW', '李设计', '#', 2, 4, '13600033333', 'li.design@qq.com', 1, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (8, 'wang_yun', '$2a$10$wxQEQUpi22a2YdS7uDCxAOtpoNuGm4Bhg05K2aAyX5OiWNNHSl4iO', '王运维-云', '#', 1, 5, '13512345678', 'wang.yun@aliyun.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (9, 'test_user', '$2a$10$Q2Tc8L8uYa1gzNcl.1s.IuZtp570gaqMbnFNUfLyFqm6xYPV.wRPu', '测试号-007', '#', 0, 6, '18999999999', 'test007@163.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (10, 'admin_test', '$2a$10$Zqr9Zx4C7Nem7M7OsQJVHOQicdpuY9bCnbnuYOKWV7dioA.HXQsam', '临时测试管理员', '#', 2, 1, '17777777777', 'temp.admin@test.org', 1, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (11, 'liu_marketing', '$2a$10$Jn9o9gs/yOvuFstdehoRbO8YNoIJKhDAZn8VKC7br.2cHw8IdecJK', '刘市场-小红', '#', 2, 7, '13333333333', 'liu.hong.marketing@dept7.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (12, 'chen_sales', '$2a$10$Lblxbpm956TrGWj/adDwgOVgIt4Z5DwVn8zN/VdsYP6iuejALxh2W', '陈销售-小明', '#', 1, 7, '13444444444', 'chen.ming.sales@dept7.org', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (13, 'secretary_wu', '$2a$10$.hilKRo.6F/QNamnBx1bW.GZmm4bz7ZcoDV0PD0rFHcrxm/BZR3ke', '吴秘书', '#', 2, 1, '13000000000', 'wu.secretary@top.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (14, 'devops_zhao', '$2a$10$qU1A2nsIQAVZLTyGXPuXfOT6Q/ZviE5fHaciTsVMWT4nM7q4kOhem', '赵运维-赵哥', '#', 1, 5, '13222222222', 'zhao.devops@dept5.net', 1, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (15, 'sun_ui', '$2a$10$4.02IteI4rp4WfrElgM2KuiKzH1itSrjexq1hS2xTcokCYQ6DEreq', '孙UI设计', '#', 2, 4, '13111111111', 'sun.ui@126.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (16, 'zhou_test', '$2a$10$g9wdXifxd4/S/hmRhpEFwOzoIldbsygDFzYzR/CWSZhJxixqRufFW', '周测试-008', '#', 1, 3, '13700000088', 'zhou008.test@dept3.cn', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (17, 'guest_user', '$2a$10$ibk6elT355.ko0LL.jZbsO860UREtKI0l0mfnc/OUatQwwer2jnl.', '访客测试员', '#', 0, 8, '18800000008', 'guest@temp.test', 1, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);

ALTER SEQUENCE "public"."sys_user_user_id_seq"
OWNED BY "public"."sys_user"."user_id";
SELECT setval('"public"."sys_user_user_id_seq"', 17, true);
ALTER TABLE "public"."sys_user" ADD CONSTRAINT "sys_user_pkey" PRIMARY KEY ("user_id");
