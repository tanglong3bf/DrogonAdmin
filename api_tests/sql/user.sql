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
  "password" varchar(20) COLLATE "pg_catalog"."default" NOT NULL,
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

INSERT INTO "public"."sys_user" VALUES (1, 'admin-1', '123456', '管理员-1', '#', 1, 1, '18888888888', 'hello@world.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (2, 'admin-2', '123456', '管理员-2', '#', 2, 1, '13666666666', 'wanan@makabaka.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (3, '123456', '123456', '超级管理员', '#', 1, 1, '13900000001', 'super@admin.com', 0, 1, '2026-04-04 08:00:00', 1, '2026-04-04 08:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (4, 'admin123', '123456', '小唐经理', '#', 2, 2, '13800000011', 'tang.manager@dept2.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (5, 'user_001', '123456', '张开发-张三', '#', 1, 3, '13700011100', 'zhang.dev@dept3.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (6, 'user_002', '123456', '张测试-李四', '#', 0, 3, '13700022200', 'zhang.qa@dept3.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (7, 'user_003', '123456', '李设计', '#', 2, 4, '13600033333', 'li.design@qq.com', 1, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (8, 'wang_yun', '123456', '王运维-云', '#', 1, 5, '13512345678', 'wang.yun@aliyun.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (9, 'test_user', '123456', '测试号-007', '#', 0, 6, '18999999999', 'test007@163.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (10, 'admin_test', '123456', '临时测试管理员', '#', 2, 1, '17777777777', 'temp.admin@test.org', 1, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (11, 'liu_marketing', '123456', '刘市场-小红', '#', 2, 7, '13333333333', 'liu.hong.marketing@dept7.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (12, 'chen_sales', '123456', '陈销售-小明', '#', 1, 7, '13444444444', 'chen.ming.sales@dept7.org', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (13, 'secretary_wu', '123456', '吴秘书', '#', 2, 1, '13000000000', 'wu.secretary@top.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (14, 'devops_zhao', '123456', '赵运维-赵哥', '#', 1, 5, '13222222222', 'zhao.devops@dept5.net', 1, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (15, 'sun_ui', '123456', '孙UI设计', '#', 2, 4, '13111111111', 'sun.ui@126.com', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (16, 'zhou_test', '123456', '周测试-008', '#', 1, 3, '13700000088', 'zhou008.test@dept3.cn', 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (17, 'guest_user', '123456', '访客测试员', '#', 0, 8, '18800000008', 'guest@temp.test', 1, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);

ALTER SEQUENCE "public"."sys_user_user_id_seq"
OWNED BY "public"."sys_user"."user_id";
SELECT setval('"public"."sys_user_user_id_seq"', 17, true);
ALTER TABLE "public"."sys_user" ADD CONSTRAINT "sys_user_pkey" PRIMARY KEY ("user_id");
